cc_binary(
    name = 'main',
    srcs = ['main.cc'],
    deps = [
        ":shader",
        "//third_party/glad:glad",
        "//third_party/stb:stb_img",
        "@glfw//:glfw",
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
    deps = ["//third_party/glad:glad"],
)
