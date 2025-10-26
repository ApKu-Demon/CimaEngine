project(CimaEngine3v)

add_subdirectory(Figuras)
add_subdirectory(Componentes)
add_subdirectory(Sistemas)

target_sources(CimaEngine3v PRIVATE Juego.cpp)
