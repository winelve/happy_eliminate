#include "usermanager.h"

QString UserManager::currentUsername = "";
int UserManager::currentUserkey;

UserManager::UserManager(QObject *parent) : QObject(parent)
{}
