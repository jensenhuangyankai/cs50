$('#password, #confirm-password').on('keyup', function () {
    var button = document.getElementById("submit");
    console.log("running");
    if ($('#password').val() == $('#confirm-password').val()) {
        button.disabled = false;
        $('#message').html('Matching').css('color', 'green');
        //console.log(button.disabled)
    } else {
        $('#message').html('Not Matching').css('color', 'red');
        button.disabled = true;
        //console.log(button.disabled)
    }});