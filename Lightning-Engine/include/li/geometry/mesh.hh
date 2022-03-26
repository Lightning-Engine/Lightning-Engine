#ifndef LI_MESH_H
#define LI_MESH_H

#include <vector>
#include <initializer_list>

namespace li {
	namespace geo {
	
		template<typename T>
		class mesh {
			std::vector<T> vertices;
			std::vector<unsigned int> indices;

		public:
			mesh() {

			}

			mesh(std::initializer_list<T> list, std::initializer_list<std::size_t> triangles)
				: vertices(list), indices(triangles) {

			}
		};

}}

#endif