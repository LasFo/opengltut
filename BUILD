cc_binary(
    name = 'main',
    srcs = ['main.cc'],
    deps = [
        ":shader",
        ":camera",
        ":model",
        "//third_party/glad:glad",
        "//third_party/stb:stb_img",
        "@glfw//:glfw",
        "@glm//:glm",
    ],
    data = [
        "shader.vs",
        "shader.fs",
        "shader.frag",
        "shader.vert",
    ] + glob(['resources/**']),
    linkopts = ["-lassimp"],
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

cc_library(
    name = 'mesh',
    hdrs = ['mesh.h'],
    deps = [
        ":shader",
        "@glm//:glm",
    ],
)

cc_library(
    name = 'model',
    hdrs = ['model.h'],
    srcs = ['model.cc'],
    deps = [
        ":shader",
        ":mesh",
        "//third_party/stb:stb_img",
        "@glm//:glm",
    ],
)
