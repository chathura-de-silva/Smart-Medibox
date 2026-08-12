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
        alarm_hours[i] = preferences.getInt((String("alarm_hours_") + String(i)).c_str(), 0); // Default(when no data in EEPROM) alarm time is 00:00. But will not trigger until user sets, since alarm_triggered is false.
        alarm_minutes[i] = preferences.getInt((String("alarm_minutes_") + String(i)).c_str(), 00);
        alarm_triggered[i] = preferences.getBool((String("alarm_triggered_") + String(i)).c_str(), false);
    }
    alarm_enabled = preferences.getBool("alarm_enabled", false);
    temp_offset_hours = preferences.getInt("utc_offset_h", DEFAULT_UTC_OFFSET_H);
    temp_offset_minutes = preferences.getInt("utc_offset_m", DEFAULT_UTC_OFFSET_M);
    wifi_username = preferences.getString("wifi_username", "");
    wifi_password = preferences.getString("wifi_password", "");
    preferences.end();
}

void save_alarm(int alarm)
{
    preferences.begin("settings", false);
    preferences.putInt((String("alarm_hours_") + String(alarm)).c_str(), alarm_hours[alarm]);
    preferences.putInt((String("alarm_minutes_") + String(alarm)).c_str(), alarm_minutes[alarm]);
    preferences.putBool((String("alarm_triggered_") + String(alarm)).c_str(), alarm_triggered[alarm]);
    preferences.end();
}

void save_time_zone()
{
    preferences.begin("settings", false);
    preferences.putInt("utc_offset_h", temp_offset_hours);
    preferences.putInt("utc_offset_m", temp_offset_minutes);
    preferences.end();
}

void save_is_alarm_enabled()
{
    preferences.begin("settings", false);
    preferences.putBool("alarm_enabled", alarm_enabled);
    preferences.end();
}

void save_wifi_credentials(String username, String password)
{
    preferences.begin("settings", false);
    preferences.putString("wifi_username", username);
    preferences.putString("wifi_password", password);
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