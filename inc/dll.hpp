#pragma once

void SetHook();
void RemoveHook();
void SetMinClickDelta(unsigned delta);

constexpr unsigned MIN_CLICK_DELTA_DEFAULT = 24;
