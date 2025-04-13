# Dependencies
This is a low dependency project but we take on dependencies for portability.
The following dependencies are needed:

* Glad: gl version 3.3+, no extensions generated from https://glad.dav1d.de/ and
  defined a `cc_library` target in //third_party/glad/BUILD with
  `strip_inlucde_prefix = "include"` to use system header style includes.
