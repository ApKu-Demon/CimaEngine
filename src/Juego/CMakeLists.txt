project(CimaEngine3v)

add_subdirectory(Figuras)
add_subdirectory(Componentes)
add_subdirectory(Sistemas)
add_subdirectory(Escenas)
add_subdirectory(Objetos)

target_sources(CimaEngine3v PRIVATE Juego.cpp)
