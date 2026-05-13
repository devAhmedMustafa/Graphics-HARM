#include "ShapeStore.h"

std::vector<std::unique_ptr<ShapeRenderer>> ShapeStore::shapes;
std::mutex ShapeStore::locker;