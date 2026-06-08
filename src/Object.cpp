#include "Object.h"
#include "FileLoader.h"
#include "Window.h"
#include <cmath>

unsigned int Object::shaderProgram = 0;
std::vector<Object*> Object::registerQueue;
std::map<int, std::vector<Object*>> Object::objects;

drawInfo::drawInfo(glm::vec2 position, glm::vec2 size)
  : position(position), size(size) {}

void Object::deletePendingObjects() {
  for (auto& [zIndex, objectsVector] : objects) {
    for (auto it = objectsVector.begin(); it != objectsVector.end(); ) {
      Object* object = *it;

      if (object->pendingDelete) {
        it = objectsVector.erase(it);
        delete object;
      }
      else {
        ++it;
      }
    }
  }
}

void Object::drawAll() {
  for (auto& [zIndex, objectsVector] : objects) {
    for (Object* object : objectsVector) {
      object->draw();
    }
  }

  deletePendingObjects();
}

void Object::updateAll() {
  for (auto& [zIndex, objectsVector] : objects) {
    for (Object* object : objectsVector) {
      object->update();
    }
  }

  deletePendingObjects();
}

void Object::registerAll() {
  for (Object* object : registerQueue) {
    objects[object->zIndex].push_back(object);
  }

  registerQueue.clear();
}

void Object::initShader() {
  // already initialized
  if (shaderProgram != 0)
      return;

  std::string vertexCode =
      FileLoader::loadFile("shaders/2DVertex.glsl");

  std::string fragmentCode =
      FileLoader::loadFile("shaders/2DFragment.glsl");

  const char* vertexShaderSource = vertexCode.c_str();
  const char* fragmentShaderSource = fragmentCode.c_str();

  // ===== VERTEX SHADER =====
  unsigned int vertexShader =
      glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(
      vertexShader,
      1,
      &vertexShaderSource,
      NULL
  );

  glCompileShader(vertexShader);

  int success;
  char infoLog[512];

  glGetShaderiv(
      vertexShader,
      GL_COMPILE_STATUS,
      &success
  );

  if (!success) {
      glGetShaderInfoLog(
          vertexShader,
          512,
          NULL,
          infoLog
      );

      std::cout
          << "VERTEX SHADER ERROR:\n"
          << infoLog
          << std::endl;
  }

  // ===== FRAGMENT SHADER =====
  unsigned int fragmentShader =
      glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(
      fragmentShader,
      1,
      &fragmentShaderSource,
      NULL
  );

  glCompileShader(fragmentShader);

  glGetShaderiv(
      fragmentShader,
      GL_COMPILE_STATUS,
      &success
  );

  if (!success) {
      glGetShaderInfoLog(
          fragmentShader,
          512,
          NULL,
          infoLog
      );

      std::cout
          << "FRAGMENT SHADER ERROR:\n"
          << infoLog
          << std::endl;
  }

  // ===== SHADER PROGRAM =====
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  glLinkProgram(shaderProgram);

  glGetProgramiv(
      shaderProgram,
      GL_LINK_STATUS,
      &success
  );

  if (!success) {
      glGetProgramInfoLog(
          shaderProgram,
          512,
          NULL,
          infoLog
      );

      std::cout
          << "PROGRAM LINK ERROR:\n"
          << infoLog
          << std::endl;
  }

  // cleanup
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  std::cout << "2D shader initialized\n";
}

void Object::init() { 
  float vertices[] = {
      // positions   // UVs
      0.0f, 0.0f,    0.0f, 0.0f,
      1.0f, 0.0f,    1.0f, 0.0f,
      1.0f, 1.0f,    1.0f, 1.0f,

      1.0f, 1.0f,    1.0f, 1.0f,
      0.0f, 1.0f,    0.0f, 1.0f,
      0.0f, 0.0f,    0.0f, 0.0f
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(vertices),
      vertices,
      GL_STATIC_DRAW
  );

  // position
  glVertexAttribPointer(
      0,
      2,
      GL_FLOAT,
      GL_FALSE,
      4 * sizeof(float),
      (void*)0
  );
  glEnableVertexAttribArray(0);

  // UV
  glVertexAttribPointer(
      1,
      2,
      GL_FLOAT,
      GL_FALSE,
      4 * sizeof(float),
      (void*)(2 * sizeof(float))
  );
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  colorChange = glm::vec3(0.0f, 0.0f, 0.0f);
  gravity = 500.0f;
  cornerRadius = 0.0f;
}

void Object::registerObject() {
  registerQueue.push_back(this);
}

Object::Object(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex) 
  : position(position), size(size), linearVelocity(glm::vec2(0.0f, 0.0f)), transparency(transparency), texture(FileLoader::loadTexture(texPath)), zIndex(zIndex), visible(true), anchored(true), canCollide(false), rotation(0), pendingDelete(false), usesColor(false) {
    init();
}

Object::Object(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex) 
  : position(position), size(size), linearVelocity(glm::vec2(0.0f, 0.0f)), transparency(transparency), color(color), zIndex(zIndex), visible(true), anchored(true), canCollide(false), rotation(0), pendingDelete(false), usesColor(true) {
    init();
}

Object::~Object() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  if (!usesColor) {
    glDeleteTextures(1, &texture);
  }
}

drawInfo* Object::beforeDrawing(drawInfo* info) {
  return info;
}

void Object::afterDrawing(drawInfo* info) {}

void Object::draw() {
  if (!visible) return;

  glUseProgram(shaderProgram);

  // ===== MODEL =====
  glm::mat4 model = glm::mat4(1.0f);

  glm::vec2 visiblePosition = position;

  drawInfo* newInfo = beforeDrawing(new drawInfo(visiblePosition, size));

  visiblePosition = newInfo->position;

  model = glm::translate(
      model,
      glm::vec3(visiblePosition, 0.0f)
  );

  // rotate around center
  model = glm::translate(
      model,
      glm::vec3(newInfo->size * 0.5f, 0.0f)
  );

  model = glm::rotate(
      model,
      glm::radians(rotation),
      glm::vec3(0.0f, 0.0f, 1.0f)
  );

  model = glm::translate(
      model,
      glm::vec3(-newInfo->size * 0.5f, 0.0f)
  );

  model = glm::scale(
      model,
      glm::vec3(newInfo->size, 1.0f)
  );

  // ===== ORTHOGRAPHIC PROJECTION =====
  glm::mat4 projection = glm::ortho(
      0.0f,
      (float)Window::fbWidth,
      (float)Window::fbHeight,
      0.0f,
      -1.0f,
      1.0f
  );

  // ===== SEND MATRICES =====
  glUniformMatrix4fv(
      glGetUniformLocation(shaderProgram, "model"),
      1,
      GL_FALSE,
      glm::value_ptr(model)
  );

  glUniformMatrix4fv(
      glGetUniformLocation(shaderProgram, "projection"),
      1,
      GL_FALSE,
      glm::value_ptr(projection)
  );

  // ===== COLOR =====
  glUniform2f(
    glGetUniformLocation(shaderProgram, "objectSize"),
    size.x, size.y
  );


  glUniform1f(
    glGetUniformLocation(shaderProgram, "cornerRadius"),
    cornerRadius
  );

  glUniform1f(
      glGetUniformLocation(shaderProgram, "alpha"),
      1 - transparency
  );

  glUniform3f(
      glGetUniformLocation(shaderProgram, "colorChange"),
      colorChange.x, colorChange.y, colorChange.z
  );

  glUniform1i(
      glGetUniformLocation(shaderProgram, "useColor"),
      usesColor ? 1 : 0
  );

  glUniform3f(
      glGetUniformLocation(shaderProgram, "color"),
      color.x, color.y, color.z
  );

  // ===== TEXTURE =====
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glUniform1i(
    glGetUniformLocation(shaderProgram, "tex"),
    0
  );

  // ===== DRAW =====
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray(0);

  afterDrawing(newInfo);

  delete newInfo;
}

void Object::beforeUpdate() {}
void Object::afterUpdate() {}

void Object::update() {
  beforeUpdate();

  if (!anchored) {  
    linearVelocity += glm::vec2(0.0f, gravity * (float)Window::deltaTime);

    position.x += linearVelocity.x * Window::deltaTime;

    if (canCollide) {
      for (auto& [zIndex, objectsVector] : objects) {
        for (Object* object : objectsVector) {
          if (!(position.x < object->position.x + object->size.x &&
                position.x + size.x > object->position.x &&
                position.y < object->position.y + object->size.y &&
                position.y + size.y > object->position.y)) continue;
          if (object == this) continue;
          if (!object->canCollide) continue;

          if (linearVelocity.x > 0.0f) {
            // moving right
            position.x = object->position.x - size.x;
          }
          else if (linearVelocity.x < 0.0f) {
            // moving left
            position.x = object->position.x + object->size.x;
          }

          linearVelocity.x = 0.0f;
        }
      }
    }

    position.y += linearVelocity.y * Window::deltaTime;

    if (canCollide) {
      for (auto& [zIndex, objectsVector] : objects) {
        for (Object* object : objectsVector) {
          if (!(position.x < object->position.x + object->size.x &&
                position.x + size.x > object->position.x &&
                position.y < object->position.y + object->size.y &&
                position.y + size.y > object->position.y)) continue;
          if (object == this) continue;
          if (!object->canCollide) continue;

          if (linearVelocity.y > 0.0f) {
            // falling
            position.y = object->position.y - size.y;
          }
          else if (linearVelocity.y < 0.0f) {
            // moving upward
            position.y = object->position.y + object->size.y;
          }

          linearVelocity.y = 0.0f;
        }
      }
    }
  }

  afterUpdate();
}

Object* Object::raycast(
  glm::vec2 rayOrigin,
  const glm::vec2& direction,
  glm::vec2& hitPoint,
  float& tHit,
  const std::vector<Object*>& ignore
) {
  Object* closestObject = nullptr;
  float closestT = 1.0f;

  for (auto& [zIndex, objectsVector] : objects) {
    for (Object* object : objectsVector) {
      if (std::find(ignore.begin(),
                        ignore.end(),
                        object) != ignore.end()) continue;
      if (!object->canCollide) continue;

      glm::vec2 min = object->position;
      glm::vec2 max = object->position + object->size;

      float tMin = 0.0f;
      float tMax = 1.0f;

      for (int i = 0; i < 2; i++) {
          float o = (i == 0) ? rayOrigin.x : rayOrigin.y;
          float d = (i == 0) ? direction.x : direction.y;
          float minVal = (i == 0) ? min.x : min.y;
          float maxVal = (i == 0) ? max.x : max.y;

          if (fabs(d) < 1e-8f)
          {
              if (o < minVal || o > maxVal)
              {
                  tMin = 2.0f;
                  break;
              }
          }
          else
          {
              float invD = 1.0f / d;
              float t1 = (minVal - o) * invD;
              float t2 = (maxVal - o) * invD;

              if (t1 > t2) std::swap(t1, t2);

              tMin = std::max(tMin, t1);
              tMax = std::min(tMax, t2);

              if (tMin > tMax)
                  break;
          }
      }

      // After slab loop
      if (tMin > tMax)
          continue;

      if (tMax < 0.0f)
          continue;

      float t = std::max(tMin, 0.0f);

      if (t <= 1.0f && t < closestT)
      {
          closestT = t;
          closestObject = object;
      }
    }
  }

  if (closestObject) {
      tHit = closestT;
      hitPoint = rayOrigin + direction * closestT; // ✅ use rayOrigin, not origin pointer
      return closestObject;
  }

  return nullptr;
}

void Object::pendDelete() {
  pendingDelete = true;
}

bool Object::isDeleted() {
  return pendingDelete;
}
