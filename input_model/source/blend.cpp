#include "blend.hpp"

#include <memory>
#include <numeric>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "camera.hpp"
#include "rtc_log.hpp"
#include "utility.hpp"

namespace rtc
{
auto release_assert(bool cond)
{
  if (!cond) std::abort();
}

struct blend::blend_impl
{
 public:
  blend_impl(const aiScene* ptr, scene_model& s) : scene{ptr}, sc{s} {}

  void validate() const;
  void load_camera();
  void load_lights();
  void load_geometry_and_materials(const aiNode*);

 private:
  const aiScene* const scene;
  scene_model& sc;

  auto get_transformation_matrix(const aiNode*);
};

auto blend::blend_impl::get_transformation_matrix(const aiNode* node)
{
  if (!node) return aiMatrix4x4{};

  auto* tmp{node};
  std::vector<aiMatrix4x4> m{};

  while (tmp)
    {
      m.push_back(tmp->mTransformation);
      tmp = tmp->mParent;
    }

  return std::accumulate(m.rbegin(), m.rend(), aiMatrix4x4{}, [](auto r, const auto& m) { return r * m; });
}

void blend::blend_impl::load_camera()
{
  const auto& camera = scene->mCameras[0];
  const auto& node = scene->mRootNode->FindNode(camera->mName);
  assert(node != nullptr);

  aiQuaternion r;
  aiVector3D pos;

  const auto t = get_transformation_matrix(node);
  t.DecomposeNoScaling(r, pos);

  const auto cam_pos = t * camera->mPosition;
  sc.optical_system.view_point = {cam_pos.x, cam_pos.y, cam_pos.z};

  const auto lookAt = r.GetMatrix() * camera->mLookAt;
  const auto up = r.GetMatrix() * camera->mUp;

  rtc::math_vector v_look = {lookAt.x, lookAt.y, lookAt.z};
  rtc::math_vector v_up = normalize({up.x, up.y, up.z});
  rtc::math_vector side = normalize(cross(v_look, v_up));

  const auto cp = camera->mClipPlaneNear;
  const auto half_horizontal_side = cp * std::tan(camera->mHorizontalFOV / 2);

  const auto aspect = camera->mAspect ? camera->mAspect : 16.0f / 9.0f;
  const auto half_vertical_side = half_horizontal_side / aspect;

  const auto screen_middle = cp * v_look + sc.optical_system.view_point;

  sc.optical_system.screen.surface.upper_left_corner =
      -half_horizontal_side * side + half_vertical_side * v_up + screen_middle;

  sc.optical_system.screen.surface.lower_left_corner =
      -(half_horizontal_side * side + half_vertical_side * v_up) + screen_middle;

  sc.optical_system.screen.surface.upper_right_corner =
      half_horizontal_side * side + half_vertical_side * v_up + screen_middle;

  sc.optical_system.screen.resolution.x = aspect * sc.optical_system.screen.resolution.y;
  RELEASE << sc.optical_system;
}

void recursive_name(const aiNode* p, int i)
{
  RELEASE << std::string(i + 3, '-') << p->mName.C_Str() << " mesh num: " << p->mNumMeshes;

  for (auto i{0U}; i < p->mNumChildren; ++i) recursive_name(p->mChildren[i], i + 1);
}

void rassert(bool f)
{
  if (!f) std::abort();
}

void blend::blend_impl::load_geometry_and_materials(const aiNode* const node)
{
  for (auto i{0U}; i < node->mNumMeshes; ++i)
    {
      const auto& mesh = scene->mMeshes[node->mMeshes[i]];
      const auto t = get_transformation_matrix(node);
      const auto offset = sc.points.size();

      for (auto v{0U}; v < mesh->mNumVertices; ++v)
        {
          const auto point{t * mesh->mVertices[v]};
          sc.points.emplace_back(point.x, point.y, point.z);
        }

      if (scene->mNumMaterials == 0) throw std::runtime_error{"Mesh has no defined material"};

      const auto& material = scene->mMaterials[mesh->mMaterialIndex];
      auto& rtc_m = sc.materials.emplace_back();
#if 1
      for (int i = 0; i < material->mNumProperties; ++i)
        {
          RELEASE << material->mProperties[i]->mKey.C_Str();
        }
#endif

      // rassert(material->Get("$mat.blend." "transparency.alpha", 0, 0,
      // rtc_m.kts) == 0); rassert(material->Get("$mat.blend." "diffuse.intensity",
      // 0, 0, rtc_m.kd) == 0); rassert(material->Get("$mat.blend."
      // "specular.intensity", 0, 0, rtc_m.ks) == 0);
      // rassert(material->Get("$mat.blend." "transparency.ior", 0, 0, rtc_m.eta)
      // == 0);

      // rtc_m.kts = 1.0f - rtc_m.kts;

      aiColor3D c1{}, c2{}, c3{}, c4{};
      material->Get(AI_MATKEY_COLOR_DIFFUSE, c1);
      material->Get(AI_MATKEY_COLOR_AMBIENT, c2);
      material->Get(AI_MATKEY_COLOR_REFLECTIVE, c3);
      material->Get(AI_MATKEY_COLOR_TRANSPARENT, c4);
      // material->Get(AI_MATKEY_BLEND, rtc_m.ks);

      rtc_m.ka = (c2.r + c2.g + c2.b) / 3;
      material->Get(AI_MATKEY_REFRACTI, rtc_m.eta);

      if (0 == material->Get(AI_MATKEY_OPACITY, rtc_m.kts))
        {
          rtc_m.kts = 1 - rtc_m.kts;
        }
      material->Get(AI_MATKEY_REFLECTIVITY, rtc_m.ks);

      rtc_m.material_color = {255 * (c1 + c2 + c3 + c4).r / 4, 255 * (c1 + c2 + c3 + c4).g / 4,
                              255 * (c1 + c2 + c3 + c4).b / 4};

      RELEASE << rtc_m;
      assert(mesh->HasFaces());

      for (auto vi{0u}; vi < mesh->mNumFaces; ++vi)
        {
          const auto& face = mesh->mFaces[vi];
          assert(face.mNumIndices == 3);

          sc.triangles.emplace_back(face.mIndices[0] + offset, face.mIndices[1] + offset, face.mIndices[2] + offset);

          sc.material_id.push_back(sc.materials.size() - 1);
        }
    }

  for (auto i{0u}; i < node->mNumChildren; ++i) load_geometry_and_materials(node->mChildren[i]);
}

void blend::blend_impl::load_lights()
{
  aiColor3D ambient;

  for (auto l{0u}; l < scene->mNumLights; ++l)
    {
      const auto& light = scene->mLights[l];
      const auto& node = scene->mRootNode->FindNode(light->mName);
      assert(node);

      const auto t = get_transformation_matrix(node);

      auto& scene_light = sc.lights.emplace_back();
      const auto conv_pos = t * light->mPosition;

      scene_light.position = {conv_pos.x, conv_pos.y, conv_pos.z};
      scene_light.light_color = {255 * (light->mColorDiffuse + light->mColorSpecular).r / 2,
                                 255 * (light->mColorDiffuse + light->mColorSpecular).g / 2,
                                 255 * (light->mColorDiffuse + light->mColorSpecular).b / 2};

      ambient = ambient + light->mColorAmbient;
    }

  sc.ambient = {ambient.r / static_cast<rtc_float>(scene->mNumLights),
                ambient.g / static_cast<rtc_float>(scene->mNumLights),
                ambient.b / static_cast<rtc_float>(scene->mNumLights)};
}

void blend::blend_impl::validate() const
{
  if (!scene->HasCameras()) throw std::runtime_error{"this scene has no camera!!"};

  if (!scene->HasLights()) throw std::runtime_error{"this scene has no lights!!"};

  if (!scene->HasMeshes()) throw std::runtime_error{"this scene has no meshes!!"};

  // if(!scene->HasMaterials())
  //  throw std::runtime_error{"this scene has no materials!!"};
}

blend::blend(const std::string& pFile)
{
  Assimp::Importer importer;
  auto scene = importer.ReadFile(pFile, aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                                            aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

  if (!scene) throw std::runtime_error{std::string{"import error: "} + importer.GetErrorString()};

  recursive_name(scene->mRootNode, 0);

  blendimpl = std::make_unique<blend_impl>(scene, *this);
  blendimpl->validate();
  blendimpl->load_geometry_and_materials(scene->mRootNode);
  blendimpl->load_lights();
  blendimpl->load_camera();
}

blend::~blend() = default;

}  // namespace rtc
