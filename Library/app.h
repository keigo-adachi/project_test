#pragma once

/// <summary>
/// AppまではCで書かれています
/// mainループの処理からは、下記の４関数が呼び出されるので、
/// この先の流れはここで書いておくこと
/// </summary>

void AppInit();

void AppUpdate();

void AppDraw();

void AppRelease();
