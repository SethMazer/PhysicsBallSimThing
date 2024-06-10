//Including libraries
#include <stdio.h>
#include <stdbool.h>
#include <raygui.h>
#include <stdlib.h>
#include <math.h>

//Struct to store spawned ball data
typedef struct {
    Vector3 position;
    float radius;
    Color color;
    bool isThrown;
    float velocityY;
    float velocityX;
    float velocityZ;

} spawnedBall;


//Create ball
spawnedBall createBall(Vector3 cameraPosition){

    //Creating ball
    spawnedBall ball;

    //Taking the vector value of camera.position passed in main and assigning to ballSpawnPosition,
    //then removing 1 from the z, so it spawns in front of user
    ball.position = cameraPosition;
    ball.position.z -= 3.0f;
    //Ball rad and colors
    ball.radius = 0.25f;
    ball.color = LIME;
    ball.isThrown = false;

    return ball;
}



// Main Function
int main(void) {

    //Creating Window
    const int windowWidth = 1600;
    const int windowHeight = 1000;
    InitWindow(windowWidth, windowHeight, "Physics Sim");

    //Camera Setup
    Camera3D camera = { 0 };
    camera.position = (Vector3){0.0f, 0.0f, 10.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;

    //Camera type
    camera.projection = CAMERA_PERSPECTIVE;
    int cameraMode = CAMERA_FIRST_PERSON;
    DisableCursor();

    //Previous camera position, needed to operate the camera coord printer on 76
    Vector3 prevCameraPosition = camera.position;

    //Other stuff
    SetTargetFPS(60);
    const float gravity = -9.81f;


    //Ball "memory"
    //Array for spawned balls
    spawnedBall * spawnedBalls = NULL;
    int spawnedBallCount = 0;


    //Main game loop
    while(!WindowShouldClose()){


        //Updating camera view/pos/etc
        UpdateCamera(&camera, cameraMode);
        //Printing camera data to terminal
        //If any camera coord isn't equal to the previous camera coord then print new camera data, this is so there isn't constant camera coord spam from just printing coords on a loop
        if((camera.position.x != prevCameraPosition.x) || (camera.position.y != prevCameraPosition.y) || (camera.position.z != prevCameraPosition.z) ){
            printf("X = %f | Y = %f | Z = %f", camera.position.x, camera.position.y,
                   camera.position.z);
            prevCameraPosition = camera.position;

        }


        //Clear Background
        ClearBackground(SKYBLUE);

        //If space is pressed call create ball function
        if(IsKeyPressed(KEY_SPACE)){
            printf("BALL CREATED");
            //Incrementing ball count
            spawnedBallCount++;
            //Resizing the spawnedBalls array, allowing balls to be stored
            spawnedBalls = realloc(spawnedBalls, (spawnedBallCount * sizeof(spawnedBall)));
            if(spawnedBalls == NULL) {
                fprintf(stderr, "Malloc failed\n");
                return 1;
            }
            //Adding a ball to the spawnedBalls array
            spawnedBalls[spawnedBallCount - 1] = createBall(camera.position);

        }





        //Physics on first ball
        //Affecting ball on key press
        if(spawnedBallCount > 0 && IsKeyPressed(KEY_P)){
            printf("<- Key Pressed -> ");

            //Ball was thrown at a velocity of 5.0f
            spawnedBalls[0].velocityY = 5.0f;
            spawnedBalls[0].velocityZ = -5.0f;
            spawnedBalls[0].isThrown = true;



        }



    //Loop
    for(int i = 0; i < spawnedBallCount; i++){
        if(spawnedBalls[i].isThrown){

            //Ball is thrown, so now ball is moving at -9.81 times GetFrameTime I think?? idk how to explain it ig
            spawnedBalls[i].velocityY += (gravity * GetFrameTime());
            //Ball is now moving at the initial velocity (5.0f) plus gravity times the frame time against Y axis
            spawnedBalls[i].position.y += (spawnedBalls[i].velocityY * GetFrameTime());
            //Ball was thrown so now its also moving along the z axis
            spawnedBalls[i].position.z += spawnedBalls[i].velocityZ * GetFrameTime();



        }



    }







        //Beginning
        BeginDrawing();
        BeginMode3D(camera);


        //Draw all balls that were stored in spawnedBalls
        for(int i = 0; i < spawnedBallCount; i++){
            DrawSphere(spawnedBalls[i].position, spawnedBalls[i].radius, spawnedBalls[i].color);
            };


        //Ending
        EndMode3D();
        EndDrawing();

    }


    free(spawnedBalls);
    CloseWindow();


}
