if(localStorage.getItem("user_id") == null || localStorage.getItem("user_id") == "null")
{
    window.location = "login.html";
}

USER_ID = parseInt(localStorage.getItem("user_id"));
