
// Texture loading code
//  stbi_set_flip_vertically_on_load(true);
//
//  unsigned int texture0;
//  glGenTextures(1, &texture0);
//  glBindTexture(GL_TEXTURE_2D, texture0);
//
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//  int width, height, nrChannels;
//  unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
//  if (data) {
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//    glGenerateMipmap(GL_TEXTURE_2D);
//  } else {
//    fprintf(stderr, "could not load texture");
//    return 1;
//  }
//  stbi_image_free(data);
//
//  unsigned int texture1;
//  glGenTextures(1, &texture1);
//  glBindTexture(GL_TEXTURE_2D, texture1);
//
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//  data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
//  if (data) {
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//    glGenerateMipmap(GL_TEXTURE_2D);
//  } else {
//    fprintf(stderr, "could not load texture");
//    return 1;
//  }
//  stbi_image_free(data);
//
//  lightingShader.use();
//  glUniform1i(glGetUniformLocation(lightingShader.ID, "texture0"), 0);
//  lightingShader.setInt("texture1", 1);
//
//
//  glm::vec3 cubePositions[] = {
//      glm::vec3(0.0f, 0.0f, 0.0f),
//      glm::vec3(2.0f, 5.0f, -15.0f),
//      glm::vec3(-1.5f, -2.2f, -2.5f),
//      glm::vec3(-3.8f, -2.0f, -12.3f),
//      glm::vec3(2.4f, -0.4f, -3.5f),
//      glm::vec3(-1.7f, 3.0f, -7.5f),
//      glm::vec3(1.3f, -2.0f, -2.5f),
//      glm::vec3(1.5f, 2.0f, -2.5f),
//      glm::vec3(1.5f, 0.2f, -1.5f),
//      glm::vec3(-1.3f, 1.0f, -1.5f)};
