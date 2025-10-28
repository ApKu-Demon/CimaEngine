project(CimaEngine3v)

add_subdirectory(Render)
add_subdirectory(App)
add_subdirectory(GUI)
add_subdirectory(Primitivos)
add_subdirectory(Utils)
add_subdirectory(Componentes)
add_subdirectory(CargarFig)
add_subdirectory(Camaras)

target_sources(CimaEngine3v PRIVATE Motor.cpp)
