/**********************************************************************************
Copyright (c) 2012 Álan Crístoffer

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
***********************************************************************************/

(function($) {

    // Define the table button
    $.cleditor.buttons.embbedimage = {
        name: "embbedimage",
        image: "embbedimage.png",
        title: "Insert Image",
        command: "inserthtml",
        buttonClick: imageButtonClick
    };

    // Add the button to the default controls
    $.cleditor.defaultOptions.controls = $.cleditor.defaultOptions.controls
    .replace("image ", "embbedimage ");

    // Table button click event handler
    function imageButtonClick(e, data) {
        var imgs = cpp.choose_and_encode_image();

        var html = '';
        for (var i = 0; i < imgs.length; i++) 
            html += '<div><img style="max-width:50%;" src="data:image/png;base64,' + imgs[i] + '" /></div>';

        data.editor.execCommand(data.command, html, null, data.button);
        data.editor.focus();
    }

})(jQuery);