file(GLOB GL_RENDERABLES_SRC
    "*.h"
    "*.cpp"
)

add_library (gl-renderables ${GL_RENDERABLES_SRC})