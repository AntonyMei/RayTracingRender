#include <utility>

//
// Created by meiyixuan on 2021-12-20.
//

#ifndef PROJECT_OBJECTPARSER_H
#define PROJECT_OBJECTPARSER_H

class ObjectParser {
public:
    ObjectParser() = delete;

    explicit ObjectParser(std::string _filename, std::string _mtl_path)
            : filename(std::move(_filename)), mtl_path(std::move(_mtl_path)) {}

    std::shared_ptr<BVHNode> parse(int bump_map_type = 0, double light_sample_probability = 0,
                                   Vector3d sun_dir = Vector3d(),
                                   const std::shared_ptr<Material> &default_material
                                   = std::make_shared<Lambertian>(1, 1, 1)) {
        // create reader
        tinyobj::ObjReaderConfig reader_config;
        reader_config.mtl_search_path = mtl_path; // Path to material files
        tinyobj::ObjReader reader;
        if (!reader.ParseFromFile(filename, reader_config)) {
            if (!reader.Error().empty()) {
                std::cerr << "TinyObjReader: " << reader.Error();
            }
            exit(1);
        }
        if (!reader.Warning().empty()) {
            std::cout << "TinyObjReader: " << reader.Warning();
        }

        // read obj data
        auto &attrib = reader.GetAttrib();
        auto &shapes = reader.GetShapes();
        auto &materials = reader.GetMaterials();

        // build material
        std::vector<std::shared_ptr<PBRMaterial>> mat_list;
        std::vector<std::shared_ptr<BumpMaterial>> bump_list;
        for (const auto &mat: materials) {
            mat_list.emplace_back(std::make_shared<PBRMaterial>(mat.name, mtl_path,
                                                                Vector3d(mat.diffuse[0],
                                                                         mat.diffuse[1],
                                                                         mat.diffuse[2]),
                                                                mat.diffuse_texname,
                                                                Vector3d(mat.specular[0],
                                                                         mat.specular[1],
                                                                         mat.specular[2]),
                                                                mat.shininess,
                                                                mat.specular_texname,
                                                                Vector3d(mat.emission[0],
                                                                         mat.emission[1],
                                                                         mat.emission[2]),
                                                                mat.emissive_texname,
                                                                mat.ior,
                                                                Vector3d(mat.transmittance[0],
                                                                         mat.transmittance[1],
                                                                         mat.transmittance[2]),
                                                                mat.dissolve,
                                                                light_sample_probability,
                                                                sun_dir));
            bump_list.emplace_back(mat.bump_texname.empty() ? nullptr :
                                   std::make_shared<BumpMaterial>(mtl_path, mat.bump_texname,
                                                                  bump_map_type));
        }

        // Loop over shapes
        HittableList mesh_list;
        for (const auto &shape: shapes) {
            // initialize storage
            size_t index_offset = 0;
            std::vector<std::shared_ptr<Hittable>> triangles;

            // Loop over triangles
            for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {

                // check vertex count
                auto fv = size_t(shape.mesh.num_face_vertices[f]);
                if (fv != 3) {
                    std::cerr << "A triangle has " << fv << " vertices." << std::endl;
                    index_offset += fv;
                    continue;
                }

                // construct vertices
                std::vector<std::shared_ptr<Vertex>> vertex_list;
                for (size_t v_id = 0; v_id < fv; v_id++) {
                    // access to vertex
                    tinyobj::index_t idx = shape.mesh.indices[index_offset + v_id];
                    tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                    tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                    tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
                    Point point(vx, vy, vz);

                    // Check if `normal_index` is zero or positive. negative = no normal data
                    Vector3d normal;
                    if (idx.normal_index >= 0) {
                        tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                        tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                        tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                        normal = Vector3d(nx, ny, nz);
                    }

                    // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                    double u = 0, v = 0;
                    if (idx.texcoord_index >= 0) {
                        tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                        tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                        u = tx;
                        v = ty;
                    }

                    // emplace vertex
                    vertex_list.emplace_back(std::make_shared<Vertex>(u, v, point, normal));
                }
                index_offset += fv;

                // per-face material (set to Lambertian for debug)
                // shape.mesh.material_ids[f];
                auto mat_id = shape.mesh.material_ids[f];
                auto mat_ptr = mat_id == -1 ? default_material : mat_list[mat_id];
                auto bump_ptr = mat_id == -1 ? nullptr : bump_list[shape.mesh.material_ids[f]];

                // create triangle
                auto triangle_ptr = std::make_shared<Triangle>(vertex_list[0], vertex_list[1],
                                                               vertex_list[2], mat_ptr, bump_ptr);
                triangles.push_back(triangle_ptr);
            }

            // build mesh
            auto mesh = std::make_shared<Mesh>(triangles);
            mesh_list.add(mesh);
        }

        // output parse statistics
        std::cerr << "*********************Parser Statistics*********************" << std::endl;
        std::cerr << "UV Mapping:" << std::endl;
        std::cerr << "Normal: " << normal_triangles << std::endl;
        std::cerr << "U-degrade: " << u_degrade_triangles << std::endl;
        std::cerr << "V-degrade: " << v_degrade_triangles << std::endl;
        std::cerr << "Unrecoverable: " << unrecoverable_triangles << std::endl;
        std::cerr << "***********************************************************" << std::endl;

        return std::make_shared<BVHNode>(mesh_list, 0.0, 1.0);
    }

private:
    std::string filename;
    std::string mtl_path;
};

#endif //PROJECT_OBJECTPARSER_H
