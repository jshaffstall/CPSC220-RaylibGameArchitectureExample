#include <vector>
#include "raylib.h"
#include "world.h"
#include "pubsub.h"
#include "entity_view.h"
#include "player_entity.h"

class Controller : public Subscriber
{
private:
    World world;
    vector<EntityView *> views;

public:
    void gameLoop();
    void receiveMessage(string channel, string message, void* data);
};

int main(void)
{
    Controller controller;
    controller.gameLoop();

    return 0;
}

void Controller::gameLoop()
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    map<KeyboardKey, PlayerAction> keyMapping;

    keyMapping[KEY_W] = PlayerUp;
    keyMapping[KEY_A] = PlayerLeft;
    keyMapping[KEY_S] = PlayerDown;
    keyMapping[KEY_D] = PlayerRight;

    PubSub::subscribe("entity", this);

    InitWindow(screenWidth, screenHeight, "Cat Moving");
    SetTargetFPS(60);

    // Load the level

    float x = 320;
    float y = 320;
    world.addPlayer(x, y, 32, 32, Cat);

    x = 160;
    world.addEntity(x, y, 32, 32, Obstacle);

    // Go into the main loop
    while (!WindowShouldClose())
    {
        // Handle user input
        for (auto &i : keyMapping)
        {
            if (IsKeyPressed(i.first))
            {
                PlayerAction action = i.second;
                PubSub::publish("action", "player", &action);
            }
        }

        // Give the world a chance to progress
        world.tick();

        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Draw the views
        for (EntityView *view : views)
            view->draw();

        EndDrawing();
    }

    CloseWindow();
}

void Controller::receiveMessage(string channel, string message, void* data)
{
    // On the "entity" channel there can be "new" and "delete" messages.
    
    if (channel == "entity" && message == "new")
    {
        // Create a view for the entity
        EntityView* view = new EntityView((Entity *) data);
        views.push_back(view);
    }

    if (channel == "entity" && message == "delete")
    {
        // Try to find the view for the entity that's being deleted
        for (int i = 0; i < views.size(); i++)
        {
            if (views[i]->isViewFor((Entity*) data))
            {
                // Delete this view's memory
                delete views[i];
                // Delete it from the vector
                views.erase(views.begin() + i);
                // and stop looking, since we found the right view
                break;
            }
        }
    }
}
