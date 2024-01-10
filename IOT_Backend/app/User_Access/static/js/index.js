let socket;
let baseurl = "http://192.168.79.8:8080/user_access";

document.getElementById('rgb').style.display = 'none';

$(document).ready(function(){
    socket = io.connect('//' + document.domain + ':' + location.port, {secure: true});

    socket.on('now_streaming', function(open_or_not){
        if(open_or_not){
            text = "streaming now";
        }
	    else{
            text = "streaming stop";
        }
        print_info(text);
    });

});

function print_info(string) {

    var textarea = document.getElementById('info');
    var text = textarea.value;

    text = text + '[INFO][RPI] ' + string + '\n';
    textarea.value = text;
    
    textarea.scrollTop = textarea.scrollHeight;
}

function print_cmd(string) {

    var textarea = document.getElementById('info');
    var text = textarea.value;

    text = text + '[CMD][RPI] ' + string + '\n';
    textarea.value = text;
    
    textarea.scrollTop = textarea.scrollHeight;
}

function update_img(){
    d = new Date();
    
    $("#rgb").attr({src: baseurl + '/rgb/' + "1_rgb.jpg"+ '/' + d.getTime()});
}

var Interval_Switch = false;
var temp_interval;

function Capture(){
    if(Interval_Switch){
        clearInterval(temp_interval);
        document.getElementById('rgb').style.display = 'none';
        number = 1;
        socket.emit('dostreaming', {'number': number, 'dostop': 0});
        Interval_Switch = false;
    }
    else{
        document.getElementById('rgb').style.display = 'block';
        //number = document.getElementById('model').value;
        temp_interval = setInterval('update_img()',10);
        number = 1;
        socket.emit('dostreaming', {'number': number, 'dostop': 1});
        Interval_Switch = true;
    }
}