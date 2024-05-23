Bullet bullet;
bullet.bulletWidth = 3;
bullet.bulletHeight = 3;
bullet.bulletPosX = player.playerPosX;
bullet.bulletPosY = player.playerPosY;
bullet.bulletSpeed = 20;

float closestDistance = std::numeric_limits<float>::max();
CPUEnemy closestEnemy;

for (size_t i = 0; i < enemies.size(); ++i) {
  if (!enemies[i].isActive) {
    continue;
  }
  float distanceToEnemy =
      sqrt(pow(player.playerPosX - enemies[i].enemyPosX, 2) +
           pow(player.playerPosY - enemies[i].enemyPosY, 2));

  if (distanceToEnemy < closestDistance) {
    closestDistance = distanceToEnemy;
    closestEnemy = enemies[i];
  }
}
float dirX = closestEnemy.enemyPosX - player.playerPosX;
float dirY = closestEnemy.enemyPosY - player.playerPosY;
float distance = sqrt(dirX * dirX + dirY * dirY);

bullet.bulletDirX = dirX / distance;
bullet.bulletDirY = dirY / distance;
bullets.push_back(bullet);
bulletCooldown = bulletCooldownDuration;