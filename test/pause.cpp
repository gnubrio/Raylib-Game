    if (IsKeyPressed(KEY_P)) {
      DrawText("Game Paused", screenWidth / 2, screenHeight / 2, 40, GRAY);
      paused = true;
      if (!paused) {
        paused = false;
        break;
      }
    }

      if (bullets[i].bulletPosX < 0 ||
          bullets[i].bulletPosX > GetScreenWidth() ||
          bullets[i].bulletPosY < 0 ||
          bullets[i].bulletPosY > GetScreenHeight()) {
        bullets.erase(bullets.begin() + i);
        --i;
      }