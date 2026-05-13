#include "Input.h"

Logger Input::logger = {};
bool Input::capturing = false;
size_t Input::nextId = 0;
std::mutex Input::callbacksMutex;
std::vector<std::pair<size_t, std::function<void(Input::MouseInput)>>> Input::mouseClickCallbacks = {};