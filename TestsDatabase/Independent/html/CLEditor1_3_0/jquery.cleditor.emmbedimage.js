/* (c) 2012 Álan Crístoffer */

(function($) {
    // Define the table button
    $.cleditor.buttons.embbedimage = {
        name: 'embbedimage',
        image: 'embbedimage.png',
        title: 'Insert Image',
        command: 'inserthtml',
        buttonClick: imageButtonClick
    };

    // Add the button to the default controls
    $.cleditor.defaultOptions.controls = $.cleditor.defaultOptions.controls
    .replace("image ", "embbedimage ");

    // Table button click event handler
    function imageButtonClick(e, data)
    {
        var input = $('<input type="file" id="files" name="files[]" multiple />');
        
        function handleFileSelect(evt) {
            var files = evt.target.files; // FileList object

            // files is a FileList of File objects. List some properties.
            var output = [];
            for (var i = 0, f; f = files[i]; i++) {
                if (!f.type.match('image.*'))
                    continue;
                
                var reader = new FileReader();

                // Closure to capture the file information.
                reader.onload = (function(theFile) {
                    return function(e) {
                        var html = '<img style="max-width:50%;" src="' + e.target.result + '" />';
                        data.editor.execCommand(data.command, html, null, data.button);
                        data.editor.focus();
                    };
                })(f);

                // Read in the image file as a data URL.
                reader.readAsDataURL(f);
            }
        }

        input.change(handleFileSelect);
        
        input.click();
    }

})(jQuery);