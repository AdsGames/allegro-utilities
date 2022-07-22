
#include "StateEngine.h"

#include "../util/Logger.h"
#include "./State.h"

// Process events
void StateEngine::processEvent(const ALLEGRO_EVENT &event)
{
  // Keyboard
  if (event.type == ALLEGRO_EVENT_KEY_DOWN ||
      event.type == ALLEGRO_EVENT_KEY_UP)
  {
    keyboardListener.onEvent(event.type, event.keyboard.keycode);
  }

  // Joystick Button
  else if (event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN ||
           event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP)
  {
    joystickListener.onEvent(event.type, event.joystick.button);
  }

  // Joystick Axis
  else if (event.type == ALLEGRO_EVENT_JOYSTICK_AXIS)
  {
    joystickListener.onEvent(event.type, event.joystick.stick,
                             event.joystick.axis, event.joystick.pos);
  }
}

// Draw
void StateEngine::draw()
{
  if (!currentState)
  {
    return;
  }

  currentState->draw();
}

// Update
void StateEngine::update(double delta)
{
  // Update listeners
  mouseListener.update();
  keyboardListener.update();
  joystickListener.update();

  if (!currentState)
  {
    return;
  }

  currentState->update(delta);

  // If the state needs to be changed
  if (currentState->getNextState() != ProgramState::NONE)
  {
    changeState(currentState->getNextState());
  }
}

// Get state id
ProgramState StateEngine::getStateId() const
{
  return stateId;
}

// Change game screen
void StateEngine::changeState(ProgramState nextState)
{
  // Delete the current state
  if (currentState != nullptr)
  {
    delete currentState;
  }

  // Change the state
  switch (nextState)
  {
  case ProgramState::EXIT:
    Logger::log("Exit state provided, exiting...");
    break;
  default:
    Logger::warn("Unknown state provided.");
    break;
  }

  currentState->init();

  // Change the current state ID
  stateId = nextState;
}
