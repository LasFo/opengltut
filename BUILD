cc_binary(
    name = 'main',
    srcs = ['main.cc'],
    deps = [
        ":shader",
        ":camera",
        "//third_party/glad:glad",
        "//third_party/stb:stb_img",
        "@glfw//:glfw",
        "@glm//:glm",
    ],
    data = [
        "shader.vert",
        "shader.frag",
        "container.jpg",
        "awesomeface.png",
    ],
)

cc_library(
    name = 'shader',
    hdrs = ['shader.h'],
    deps = [
        "//third_party/glad:glad",
        "@glm//:glm",
    ],
)

cc_library(
    name = 'camera',
    hdrs = ['camera.h'],
    deps = [
        "//third_party/glad:glad",
        "@glm//:glm",
    ],
)
