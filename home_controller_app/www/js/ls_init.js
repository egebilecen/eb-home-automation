// Initialize localstorage variables.
// This file must only included in login.html.
if(localStorage.getItem("temp_sensor_place_id") == null)
{
    localStorage.setItem("temp_sensor_place_id", 1);
}

if(localStorage.getItem("temp_chart_min_val") == null)
{
    localStorage.setItem("temp_chart_min_val", -40);
}

if(localStorage.getItem("temp_chart_max_val") == null)
{
    localStorage.setItem("temp_chart_max_val", 110);
}

if(localStorage.getItem("temp_chart_data_limit") == null)
{
    localStorage.setItem("temp_chart_data_limit", 5);
}

if(localStorage.getItem("auto_login") == null)
{
    localStorage.setItem("auto_login", 0);
}

localStorage.setItem("user_id", null);
