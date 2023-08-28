#include "raylib.h"
#include <math.h>

const Vector2 screen_dimensions = (Vector2){520, 300};

typedef struct {
    Vector2 position;
    float height;
    float zfar;
} CAMERA;

CAMERA camera = {
    .position = (Vector2){512., 512.},
    .height = 100,
    .zfar = 800,
};

void Draw_Terrain(Image *color_image, Image *height_image, Vector2 image_size, int scale_factor) {
    Vector2 leftPoint = (Vector2){-camera.zfar, camera.zfar};
    Vector2 rightPoint = (Vector2){camera.zfar, camera.zfar};

    for (int i=0; i<screen_dimensions.x; i++) {
        Vector2 delta = (Vector2){
            (leftPoint.x + (rightPoint.x - leftPoint.x) / screen_dimensions.x * i) / camera.zfar,
            (leftPoint.y + (rightPoint.y - leftPoint.y) / screen_dimensions.x * i) / camera.zfar,
        };
        Vector2 ray = camera.position;

        int max_height = screen_dimensions.y;
        for (int z=1; z<camera.zfar; z++) {
            ray.x += delta.x;
            ray.y += delta.y;

            if (!(ray.x > image_size.x-1 || ray.x < 0 || ray.y > image_size.y-1 || ray.y < 0)) { // if out of bounds
                Color color = GetImageColor(*color_image, ray.x, ray.y);
                int height = (camera.height - GetImageColor(*height_image, ray.x, ray.y).r) / z * scale_factor;

                if (height < 0) {
                    height = 0;
                } else if (height > screen_dimensions.y) {
                    height = screen_dimensions.y - 1;
                }

                if (height < max_height) {
                    for (int y=height; y < max_height; y++) {
                        DrawPixel(i, y, color);
                    }

                    max_height = height;
                }
            }
        }
    }
}

int main(void)
{
    InitWindow(screen_dimensions.x, screen_dimensions.y, "raylib [core] example - basic window");
    Image color_image = LoadImage("maps/color_map.png");
    Image height_image = LoadImage("maps/height_map.png");
    Texture2D color_texture = LoadTextureFromImage(color_image);
    Texture2D height_texture = LoadTextureFromImage(height_image);

    while (!WindowShouldClose())
    {
        
        if (IsKeyDown(KEY_W)) {
            camera.position.y++;
        } else if (IsKeyDown(KEY_S)) { 
            camera.position.y--;
        }
        if (IsKeyDown(KEY_D)) {
            camera.position.x++;
        } else if (IsKeyDown(KEY_A)) { 
            camera.position.x--;
        }
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            camera.height--;
        } else if (IsKeyDown(KEY_SPACE)) {
            camera.height++;
        }

        if (IsKeyDown(KEY_E)) {
            ImageDrawCircle(&height_image, camera.position.x, camera.position.y + 30, 10, (Color){camera.height-20, camera.height-20, camera.height-20, 255});
        }

        BeginDrawing();
            ClearBackground(BLACK);

            DrawText(TextFormat("FPS: %d", GetFPS()), 0, 0, 20, WHITE);

            Draw_Terrain(&color_image, &height_image, (Vector2){1024, 768}, 100);
        EndDrawing();
    }

    CloseWindow();
    UnloadImage(color_image); UnloadImage(height_image);
    UnloadTexture(color_texture); UnloadTexture(height_texture);

    return 0;
}