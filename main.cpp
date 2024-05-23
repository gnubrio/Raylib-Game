#include <cmath>
#include <iostream>
#include <raylib.h>
#include <vector>

class Player {
public:
  int health;
  float positionX, positionY;
  float speedX, speedY;
  int radius;

  void Draw() { DrawCircle(positionX, positionY, radius, BEIGE); }

  void Update() {
    if (IsKeyDown(KEY_W) && positionY - speedY >= radius) {
      positionY -= speedY;
    }
    if (IsKeyDown(KEY_S) && positionY + speedY <= GetScreenHeight() - radius) {
      positionY += speedY;
    }
    if (IsKeyDown(KEY_A) && positionX - speedX >= radius) {
      positionX -= speedX;
    }
    if (IsKeyDown(KEY_D) && positionX + speedX <= GetScreenWidth() - radius) {
      positionX += speedX;
    }
  }
  void DecreaseHealth(int amount) { health -= amount; }

  void IncreaseHealth(int amount) { health += amount; }

  int GetHealth() { return health; }
};
class CPUZombie {
public:
  float positionX, positionY;
  int speedX, speedY;
  int radius;
  bool isActive;

  void Draw() { DrawCircle(positionX, positionY, radius, DARKGREEN); }

  void DrawInactive() { DrawCircle(positionX, positionY, radius, RED); }

  void Update(float playerPosX, float playerPosY) {
    float distanceX = playerPosX - positionX;
    float distanceY = playerPosY - positionY;
    float distance = sqrt(distanceX * distanceX + distanceY * distanceY);

    if (distance != 0) {
      distanceX /= distance;
      distanceY /= distance;
    }
    positionX += distanceX * speedX;
    positionY += distanceY * speedY;
  }
};
class Bullet {
public:
  float positionX, positionY;
  float height, width;
  float directionX, directionY;
  int speed;

  void Draw() { DrawRectangle(positionX, positionY, width, height, YELLOW); }

  void Update() {
    positionX += directionX * speed;
    positionY += directionY * speed;
  }
};
class Medkit {
public:
  int value;
  float positionX, positionY;
  float width, height;
  bool isActive;

  void Draw() { DrawRectangle(positionX, positionY, width, height, WHITE); }
};
class Ammocase {
public:
  float value;
  float positionX, positionY;
  float width, height;
  bool isActive;

  void Draw() { DrawRectangle(positionX, positionY, width, height, ORANGE); }
};
int main() {
  const int screenWidth = 1900;
  const int screenHeight = 1060;

  float timeElapsed = 0.0f;
  bool gameOver = false;

  InitWindow(screenWidth, screenHeight, "Scoopa Troopa");
  SetTargetFPS(60);

  Player player;
  player.health = 100;
  player.radius = 10;
  player.positionX = screenWidth / 2;
  player.positionY = screenHeight / 2;
  player.speedX = 1.5;
  player.speedY = 1.5;

  CPUZombie zombie;
  std::vector<CPUZombie> zombies;
  float zombieSpawnTimer = 0.0f;
  float zombieSpawnInterval = 7.0f;
  int zombieSpawnCount = 1;
  int maxZombieSpawnCount = 10;

  Bullet bullet;
  std::vector<Bullet> bullets;
  float fireRateTimer = 0.0f;
  float fireRate = 2.0f;

  Medkit medkit;
  float medkitSpawnTimer = 0.0f;
  float medkitSpawnInterval = 20.0f;
  medkit.isActive = false;

  Ammocase ammo;
  float ammoSpawnTimer = 0.0f;
  float ammoSpawnInterval = 45.0f;
  ammo.isActive = false;

  while (!gameOver && !WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(DARKGRAY);

    medkitSpawnTimer += GetFrameTime();
    if (medkitSpawnTimer >= medkitSpawnInterval && (!medkit.isActive)) {
      medkit.value = GetRandomValue(15, 45);
      medkit.positionX = GetRandomValue(200, screenWidth - 200);
      medkit.positionY = GetRandomValue(200, screenHeight - 200);
      medkit.width = 10;
      medkit.height = 10;
      medkit.isActive = true;
      medkitSpawnTimer = 0.0f;
    }
    if (medkit.isActive) {
      if (CheckCollisionCircleRec(Vector2{player.positionX, player.positionY},
                                  player.radius,
                                  Rectangle{medkit.positionX, medkit.positionY,
                                            medkit.width, medkit.height})) {
        player.IncreaseHealth(medkit.value);
        medkit.isActive = false;
      }
    }
    ammoSpawnTimer += GetFrameTime();
    if (ammoSpawnTimer >= ammoSpawnInterval && (!ammo.isActive)) {
      ammo.value = 0.3f;
      ammo.positionX = GetRandomValue(200, screenWidth - 200);
      ammo.positionY = GetRandomValue(200, screenHeight - 200);
      ammo.width = 40;
      ammo.height = 20;
      ammo.isActive = true;
      ammoSpawnTimer = 0.0f;
    }
    if (ammo.isActive) {
      if (CheckCollisionCircleRec(Vector2{player.positionX, player.positionY},
                                  player.radius,
                                  Rectangle{ammo.positionX, ammo.positionY,
                                            ammo.width, ammo.height})) {
        fireRate -= ammo.value;
        ammo.isActive = false;
      }
    }
    zombieSpawnTimer += GetFrameTime();
    if (zombieSpawnTimer >= zombieSpawnInterval) {
      zombieSpawnCount++;
      if (zombieSpawnCount >= maxZombieSpawnCount) {
        zombieSpawnCount = zombieSpawnCount / 2;
        zombieSpawnInterval -= 1.0f;
        maxZombieSpawnCount += 1;
        if (zombieSpawnInterval < 1.0f) {
          zombieSpawnInterval = 5.0f;
        }
      }
      for (int i = 0; i < zombieSpawnCount; ++i) {
        zombie.radius = 8;
        int side = GetRandomValue(0, 3);
        switch (side) {
        case 0:
          zombie.positionX = GetRandomValue(0, screenWidth);
          zombie.positionY = -zombie.radius;
          break;
        case 1:
          zombie.positionX = screenWidth + zombie.radius;
          zombie.positionY = GetRandomValue(0, screenHeight);
          break;
        case 2:
          zombie.positionX = GetRandomValue(0, screenWidth);
          zombie.positionY = screenHeight + zombie.radius;
          break;
        case 3:
          zombie.positionX = -zombie.radius;
          zombie.positionY = GetRandomValue(0, screenHeight);
          break;
        }
        zombie.speedX = 1;
        zombie.speedY = 1;
        zombie.isActive = true;
        zombies.push_back(zombie);
        zombieSpawnTimer = 0.0f;
      }
    }
    fireRateTimer += GetFrameTime();
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && fireRateTimer >= fireRate) {
      bullet.width = 3;
      bullet.height = 3;
      bullet.positionX = player.positionX;
      bullet.positionY = player.positionY;
      bullet.speed = 15;

      Vector2 mousePosition = GetMousePosition();
      float directionX = mousePosition.x - player.positionX;
      float directionY = mousePosition.y - player.positionY;
      float distance = sqrt(directionX * directionX + directionY * directionY);

      bullet.directionX = directionX / distance;
      bullet.directionY = directionY / distance;
      bullets.push_back(bullet);
      fireRateTimer = 0.0f;
    }
    for (size_t i = 0; i < bullets.size(); ++i) {
      bullets[i].Update();
      bullets[i].Draw();
      if (bullets[i].positionX < 0 || bullets[i].positionX > GetScreenWidth() ||
          bullets[i].positionY < 0 ||
          bullets[i].positionY > GetScreenHeight()) {
        bullets.erase(bullets.begin() + i);
        --i;
      }
    }
    for (int i = bullets.size() - 1; i >= 0; --i) {
      for (size_t j = 0; j < zombies.size(); ++j) {
        if (zombies[j].isActive &&
            CheckCollisionCircleRec(
                Vector2{zombies[j].positionX, zombies[j].positionY},
                zombies[j].radius,
                Rectangle{bullets[i].positionX, bullets[i].positionY,
                          bullets[i].width, bullets[i].height})) {
          zombies[j].isActive = false;
        }
      }
    }
    for (size_t i = 0; i < zombies.size(); ++i) {
      if (zombies[i].isActive) {
        zombies[i].Update(player.positionX, player.positionY);
        zombies[i].Draw();

        if (CheckCollisionCircles(
                Vector2{zombies[i].positionX, zombies[i].positionY},
                zombies[i].radius, Vector2{player.positionX, player.positionY},
                player.radius)) {
          player.DecreaseHealth(1);
        }
      } else {
        zombies[i].DrawInactive();
        zombies[i].radius = 6;
        zombies[i].speedX = 0;
        zombies[i].speedY = 0;
      }
    }
    player.Update();
    player.Draw();
    if (medkit.isActive)
      medkit.Draw();
    if (ammo.isActive)
      ammo.Draw();

    int currentHealth = player.GetHealth();
    if (currentHealth > 100) {
      currentHealth = 100;
    }
    DrawText(TextFormat("Health: %d", currentHealth), 20, 10, 30, BLUE);
    if (currentHealth <= 0) {
      gameOver = true;
      break;
    }
    timeElapsed = GetTime();
    DrawText(TextFormat("Time alive: %.2f", timeElapsed), 20, 40, 20, BLACK);
    EndDrawing();
  }
  while (!WindowShouldClose()) {
    float endingTime = timeElapsed;
    BeginDrawing();
    DrawText("GAME OVER!", screenWidth / 2 - MeasureText("GAME OVER!", 50) / 2,
             screenHeight / 2 - 50, 50, BLACK);
    DrawText("Press ESC to quit",
             screenWidth / 2 - MeasureText("Press ESC to quit", 20) / 2,
             screenHeight / 2 + 80, 20, BLACK);
    DrawText(TextFormat("You survived for %.2f seconds.", endingTime),
             (screenWidth / 2) - 233, screenHeight / 2 + 20, 30, BLACK);
    EndDrawing();

    if (IsKeyDown(KEY_ESCAPE)) {
      break;
    }
  }
  CloseWindow();
  return 0;
}
