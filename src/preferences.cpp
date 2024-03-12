#include <Preferences.h>
#include <Globals.h>
#include <Constants.h>
#include <Functions.h>

Preferences preferences;

void load_user_settings()
{
    preferences.begin("settings", true);
    for (int i = 0; i < n_alarms; i++)
    {
        alarm_hours[i] = preferences.getInt("alarm_hours_" + i, 0); // default alarms set at ,1,2,3.
        alarm_minutes[i] = preferences.getInt("alarm_minutes_" + i, 00);
        alarm_triggered[i] = preferences.getBool("alarm_triggered_" + i, false);
    }
    alarm_enabled = preferences.getBool("alarm_enabled", false);
    utc_offset = preferences.getInt("utc_offset", DEFAULT_UTC_OFFSET);
    preferences.end();
}

void save_alarm(int alarm)
{
    preferences.begin("settings", false);
    preferences.putInt("alarm_hours_" + alarm, alarm_hours[alarm]);
    preferences.putInt("alarm_minutes_" + alarm, alarm_minutes[alarm]);
    preferences.putBool("alarm_triggered_" + alarm, alarm_triggered[alarm]);
    preferences.end();
}

void save_time_zone()
{
    preferences.begin("settings", false);
    preferences.putInt("utc_offset", utc_offset);
    preferences.end();
}

void save_is_alarm_enabled()
{
    preferences.begin("settings", false);
    preferences.putBool("alarm_enabled", alarm_enabled);
    preferences.end();
}

void reset_preferences()
{
    preferences.begin("settings", false);
    preferences.clear();
    preferences.end();
    show_modal_page(reset, 300, "Resetting Medibox!", 16);
    ESP.restart();
}