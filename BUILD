cc_binary(
    name = 'main',
    srcs = ['main.cc'],
    deps = [
        ":shader",
        "//third_party/glad:glad",
        "@glfw//:glfw",
    ],
    data = [
        "shader.vert",
        "shader.frag"
    ],
)

cc_library(
    name = 'shader',
    hdrs = ['shader.h'],
    deps = ["//third_party/glad:glad"],
)
