function redirect(url)
{
    //Mimic link click animation
    $("body").append("<a id='mainpage-link' href='"+url+"' style='display:none;'></a>");
    $("#mainpage-link").click();
}

function get_url_as_json(url) 
{
    if(!url) url = location.search;
    var query = url.substr(1);
    var result = {};
    query.split("&").forEach(function(part) {
      var item = part.split("=");
      result[item[0]] = decodeURIComponent(item[1]);
    });
    return result;
}

function get_url_param(param)
{
    return get_url_as_json()[param];
}

function parse_mysql_date(_date)
{
    var date_delimiter = "/";
    var hour_delimiter = ", ";
    
    return _date.split(" ")[0].split("-")[2] + date_delimiter + _date.split(" ")[0].split("-")[1] + date_delimiter + _date.split(" ")[0].split("-")[0] + hour_delimiter + _date.split(" ")[1].split(":")[0] + ":" + _date.split(" ")[1].split(":")[1];
}



function add_all_readed_notification()
{
    var html_data =  '   <div class="notifi__item">  '  +  
                             '       <div class="bg-c1 img-cir img-40" style="background:#cecece;">  '  + 
                             '           <i class="fa fa-check"></i>  '  + 
                             '       </div>  '  + 
                             '       <div class="content">  '  + 
                             '           <p>Bütün bildirimler okundu.</p>  '  + 
                             '           <span class="date"></span>  '  + 
                             '       </div>  '  + 
                             '  </div>  ' ; 
    $(".notifi-dropdown.js-dropdown").prepend(html_data);
}
