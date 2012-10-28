/* (c) 2012 Álan Crístoffer */

var App = {};

$(document).ready(function() {
    available_languages();
    
	App.category = 0;
    App.state = 'category-show';
	checkDB();
	
	$("#question-form-difficulty").slider({animate:true, max:10, value:6});
    $("#test-form-difficulty").slider({animate:true, max:10, values:[0,6], range:true});
    
    $("#test-form-number-questions").spinner({min:0,max:50});
    $("#test-form-number-tests").spinner({min:1});
	
    $("textarea").cleditor({width: 600, height: 400});
    
	setup_events();
	refreshUI();
});

function available_languages()
{
    var langs = JSON.parse(cpp.get_languages());
    langs['English'] = 'en';

    var ordered_langs = [];

    for (key in langs) {
        ordered_langs.push(key);
    };
    
    ordered_langs.sort();

    var html = '';
    
    for (var i=0; i < ordered_langs.length; i++) {
        var key = ordered_langs[i];
        html += '<li><a data-lang="' + langs[key] + '">' + key + '</a></li>';
    }

    $("#language-dropdown").html(html);
    
	var elements = $("*").filter(function() {
		return $(this).data("lang") != undefined; 
	});
    
    $(elements).click(function() {
        cpp.set_language($(this).data('lang'));
        translate();
    });
}

function close_modals()
{
	$(".reveal-modal").trigger("reveal:close");
}

function setup_events()
{
	$(".close-modal").click(function(){
		close_modals();
	});
	
	$("#category-new").click(function(){
		$("#category-new-modal").reveal();
		$("#category-new-modal-input").focus();
	});
	
	$("#category-new-modal-save").click(function(){
		var name = $("#category-new-modal-input").val();
		$("#category-new-modal-input").val('');
		cpp.category_new(name, App.category);
		close_modals();
		refreshUI();
	});
    
    $("#category-new-modal-input").keypress(function(event) {
        if ( event.which == 13 ) {
            $("#category-new-modal-save").click();
        }
    });
	
	$("#category-edit").click(function(){
		App.state = "category-edit";
		refreshUI();
	});
	
	$("#category-edit-form-save").click(function(){
		var name = $("#category-edit-form-name").val();
		var parent = $("input[name=category-edit-form-parent]:checked").val();
		cpp.category_update(App.category, name, parent);
        App.state = 'category-show';
		refreshUI();
	});
	
	$("#category-edit-form-merge").click(function(){
		$("#category-merge-modal").reveal();
	});
	
	$("#category-merge-modal-comfirm").click(function(){
		close_modals();
		var cat = $("input[name=category-edit-form-parent]:checked").val();
		cpp.category_merge(App.category, cat);
		App.category = cat;
		refreshUI();
	});
	
	$("#category-edit-form-delete").click(function(){
		$("#category-delete-modal").reveal();
	});
	
	$("#category-delete-modal-comfirm").click(function(){
		close_modals();
		cpp.category_delete(App.category);
		App.category = 0;
		refreshUI();
	});
	
	$("#open-dialog-new").click(function() {
		cpp.create_and_open_db();
	});
	
	$("#open-dialog-open").click(function() {
		cpp.choose_and_open_db();
	});
	
	$("#question-new").click(function() {
		App.state = 'question-new';
		$("#question-form-id").data('id', -1);
		refreshUI();
	});
	
	$("#question-form-difficulty" ).bind( "slidechange", function(event, ui) {
		$("#question-form-difficulty-span").html(ui.value);
	});
	
	$('#question-form-save').click(function() {
		var id = $("#question-form-id").data('id');
		var title = $("#question-form-title").val();
		var difficulty = $('#question-form-difficulty').slider('value');
		var reference = $("#question-form-reference").val();
		var body = $("#question-form-body").val();
		var category = $('input[name=category-tree-radio]:checked').val();
        
        if ( title == '' ) {
            $('#no-title-modal').reveal(); return;
        }
		
		var answers = [];
		var right_answer = null;
		
		$('input[name^=question-form-answers-fields]').each(function() {
			var r = $(this).prev('input').is(':checked');
			if ( r ) {
				right_answer = $(this).val();
			} else {
				answers.push($(this).val());
			}
		});
		
		if ( right_answer == null ) {
			$('#question-modal-no-right-answer').reveal();
			return;
		}
		
		if ( id < 0 ) {
			id = cpp.question_new(title, reference, difficulty, body, category);
			
			cpp.answer_new(right_answer, id, true);
			for (var i = 0; i < answers.length; i++ ) {
				cpp.answer_new(answers[i], id, false);
			}
		} else {
			cpp.question_update(id, title, reference, difficulty, body, category);
			
			cpp.answer_delete(id);
			cpp.answer_new(right_answer, id, true);
			for (var i = 0; i < answers.length; i++ ) {
				cpp.answer_new(answers[i], id, false);
			}
		}
		
		App.category = category;
		App.question = id;
		App.state = 'question-view';
		refreshUI();
	});
	
	$('#question-view-delete').click(function() {
		$('#question-modal-delete').reveal();
	});
	
	$("#question-modal-delete-comfirm").click(function() {
		if ( App.question >= 0) cpp.question_delete(App.question);
		App.question = -1;
		App.state = 'category-show';
		close_modals();
		refreshUI();
	});
	
	$("#question-view-edit").click(function() {
		App.state = 'question-edit';
		$("#question-form-id").data('id', App.question);
		refreshUI();
	});
    
    $('#about').click(function() {
        $('#app-about').reveal();
    });
    
    $('#about-qt').click(function() {
        cpp.about_qt();
    });
    
    $('#header-menu-new').click(function() {
        cpp.create_and_open_db();
    });
    
    $('#header-menu-open').click(function() {
        cpp.choose_and_open_db();
    });
    
    $('#header-menu-show-categories').click(function() {
        App.state = 'category-show';
        refreshUI();
    });
    
    $('#header-menu-show-tests').click(function() {
        App.state = 'test-list';
        refreshUI();
    });
    
    $('#test-new').click(function() {
        App.state = 'test-new';
        refreshUI();
    });
    
	$("#test-form-difficulty" ).bind( "slidechange", function(event, ui) {
		$("#test-form-difficulty-min-span").html($(this).slider('values', 0));
        $("#test-form-difficulty-max-span").html($(this).slider('values', 1));
	});
    
    $('.paper-size').click(function() {
        $("#test-form-page-width").val($(this).data('width'));
        $("#test-form-page-height").val($(this).data('height'));
    });
    
    $('#test-form-save').click(function() {
        create_test();
    });
    

    $('#test-show-delete').click(function() {
        $('#test-modal-delete').reveal();
    });
    
    $('#test-modal-delete-comfirm').click(function(){
        cpp.test_delete(App.test);
        App.test = -1;
        App.state = 'test-list';
        refreshUI();
        close_modals();
    });
    
    $('#test-show-gen-pdf').click(function() {
        cpp.print('pdf', cpp.test(App.test)[2]);
    });
    
    $('#test-show-gen-htm').click(function() {
        cpp.print('html', cpp.test(App.test)[2]);
    });
}

function checkDB()
{
	if( !cpp.hasDataBase() ) {
		openFileDialog();
	} else {
		closeFileDialog();
	}
	refreshUI();
}

function openFileDialog()
{
	$("#open-dialog").reveal({closeonbackgroundclick: false});
}

function closeFileDialog()
{
	$("#open-dialog").trigger("reveal:close");
}

function refreshUI()
{
	refreshContents();
	refreshNavbar();
	refreshMenu();
	translate();
}

function refreshContents()
{
    // memory management. It can contain very large data
    $("#question-form-body").val('');
    $('#question-view-body').val('');
    cpp.clear_caches();
    
	$("#category-edit-wrapper").hide();
	$("#question-form-wrapper").hide();
	$("#question-view").hide();
    $('#test-form').hide();
    $('#test-show').hide();

	switch( App.state ) {
		case 'category-edit':
		if (App.category == 0) return;
		category_edit_prepare();
		$("#category-edit-wrapper").show('fade');
        $("#category-edit-form-name").focus();
		break;
		
		case 'question-edit':
		$('#question-add-span').hide();
		$('#question-edit-span').show();
		question_edit_prepare();
		$("#question-form-wrapper").show('fade');
        $("#question-form-title").focus();
		break;
		
		case 'question-new':
		$('#question-add-span').show();
		$('#question-edit-span').hide();
		question_edit_prepare();
		$("#question-form-wrapper").show('fade');
        $("#question-form-title").focus();
		break;
		
		case 'question-view':
		question_show_prepare();
		$("#question-view").show('fade');
		break;
        
        case 'test-new':
        prepare_test_form();
        $('#test-form').show('fade');
        $("#test-form-title").focus();
        break;
        
        case 'test-show':
        test_show_prepare();
        $('#test-show').show();
        break;
	}
}

function show_category_navbar() {
    $('#category-edit-btn-toolbar').show();
	if ( App.category == 0 ) $("#category-edit").hide();
	else $("#category-edit").show();
	
	// list categories in the left panel
	var children = JSON.parse(cpp.category_children(App.category));
	var html = '';
    var categories = '';
    var questions = '';
	for (var i = 0; i < children.length; i++) {
		var child = children[i];
        var icon = '';
        if (child.kind=='category') {
            icon = '<i class="icon-folder-close"></i>';
            categories += "<li data-id='" + child.id + "' data-kind='" + child.kind + "'><a>" + icon + child.name + "</a></li>";
        } else {
            questions += "<li data-id='" + child.id + "' data-kind='" + child.kind + "'><a>" + child.name + "</a></li>";
        }
    }
    
    if ( categories.length ) html += "<li class='nav-header' data-text='Categories'></li>";
    html += categories;
    
    if ( questions.length ) html += "<li class='nav-header' data-text='Questions'></li>";
    html += questions;
	
    $("#categories-list").html(html);
	
	// change the path in the top widget
	if ( App.category == 0 ) {
		$("#path-navigator").hide();
	} else {
		$("#path-navigator").show();
		var arr = [];
		var cat = cpp.category(App.category);
		var sep = "";
		while (true) {
			arr.push("<li><a data-id='" + cat[0] + "'>" + cat[1] + "</a>" + sep + "</li>");
			if ( cat[2] == 0 ) break;
			cat = cpp.category(cat[2]);
			sep = "<span class='divider'>/</span>";
		}
		arr.push("<li><a data-id='0' data-text='Root'></a><span class='divider'>/</span></li>");
		arr.reverse();
		var html = "";
		for (var i = 0; i < arr.length; i++) {
			html += arr[i];
		}
		$("#path-navigator ul").html(html);
	}

	$("#categories-list li").click(function(){
		var id   = $(this).data('id');
		var kind = $(this).data('kind');

		if ( kind == 'category' ) {
			App.state = 'category-show';
			App.category = id;
            $("#categories-list").parent().hide("slide",{},200, refreshUI).show("slide", { direction: "right" },200);
		} else {
			App.state = 'question-view';
			App.question = id;
            refreshUI()
		}
        
        $('body').scrollTop(0);
	});
	
	$("#path-navigator li a").click(function(){
		App.category = $(this).data('id');
		App.state = 'category-show';
        $("#categories-list").parent().hide("slide", { direction: "right" },200, refreshUI).show("slide",{},200);
	});
};

function show_tests_navbar()
{
    $('#test-btn-toolbar').show();
    $('#categories-list').html('loading');
    var tests = JSON.parse(cpp.test_all());
    var html = '';
    for (var i=0; i < tests.length; i++) {
        var t = tests[i];
        html +=  '<li data-id="' + t[0] + '"><a>' + t[1] + '</a></li>';
    }
    $('#categories-list').html(html);
    
    $('#categories-list li').click(function() {
        App.test = $(this).data('id');
        App.state = 'test-show';
        refreshUI();
    });
}

function refreshNavbar()
{
    $("#path-navigator").hide();
    $('#category-edit-btn-toolbar').hide();
    $('#test-btn-toolbar').hide();
    
    switch ( App.state ) {
		case 'category-edit':
		case 'question-edit':
		case 'question-new':
		case 'question-view':
        case 'category-show':
        show_category_navbar();
		break;
        
        case 'test-list':
        case 'test-show':
        case 'test-new':
        show_tests_navbar();
        break;
    }
}

function question_show_prepare()
{
	var q = cpp.question(App.question);
	
	$('#question-view-title').html(q[1]);
	$('#question-view-reference').html('<b data-text="Reference: "></b>' + q[4]);
	$('#question-view-difficulty').html('<b data-text="Difficulty: "></b>' + q[3]);
	$('#question-view-body').html(q[2]);
	
	var as = JSON.parse(cpp.answer_all(q[0]));
	var html = '';
	for (var i = 0; i < as.length; i++) {
		var r = '';
		if ( as[i].right ) r = 'question-view-answer-right';
		html += '<li class="' + r + '">' + as[i].body + '</li>';
	}
	$('#question-view-answers').html(html);
};

function refreshMenu()
{
	
}

function translate()
{
	/*
	The lines marked with "ease translation process"
	are used to ouput the content of the html 'section'
	of UIBridge::htmlTranslations() so I don't have to
	type all the tr() lines that are used by lupdate
	and thus can be removed if necessary (production, for example)
	*/
	var elements = $("*").filter(function() {
		return $(this).data("text") != undefined; 
	});
    
	var trs = []; // ease translation process
	
    for( var i = 0; i < elements.length; i++) {
		var e = elements[i];
		
        trs.push($(e).data('text')); // ease translation process

		if ( $(e).html ) {
			$(e).html(cpp.jstr($(e).data('text')));
		} else if ( $(e).val ) {
			$(e).val(cpp.jstr($(e).data('text')));
		}
	}
    
    add_icons();
    
    // ease translation process
    var utrs = [];
    $.each(trs, function(i, el){
        if($.inArray(el, utrs) === -1) utrs.push(el);
    });
    
    var str = '';
    for (var i=0; i < utrs.length; i++) {
        var tr = utrs[i];
        str += 'tr("' + tr + '");\n';
    };
    
    //cpp.debug(str);
}

function add_icons()
{
	var elements = $("*").filter(function() {
		return $(this).data("icon") != undefined; 
	});
	for( var i = 0; i < elements.length; i++) {
        var e = elements[i];
        
        $(e).html( '<i class="icon-' + $(e).data('icon') + '"></i> ' + $(e).html());
    }
}

function html_category_tree(id, skip)
{
	if ( id == skip ) return '';
	var cat = cpp.category(id);
	var html = "<div><label class='radio'><input id='category-edit-radio-" + cat[0] + "' type='radio' value='" + cat[0] + "' name='category-edit-form-parent'> " + cat[1] + "</label>";
	
	var children = JSON.parse(cpp.category_children(id));
	for (var i = 0; i < children.length; i++ ) {
		var child = children[i];
		if ( child.kind == 'category' ) html += html_category_tree(child.id, skip);
	}
	
	html += "</div>";
	return html;
}

function category_edit_prepare()
{
	var cat = cpp.category(App.category);
	$("#category-edit-form-name").val(cat[1]);

	var html = "<div><label class='radio'><input type='radio' id='category-edit-radio-0' value='0' name='category-edit-form-parent'> " + cpp.jstr("Root") + "</label>";
	var children = JSON.parse(cpp.category_children(0));
	for (var i = 0; i < children.length; i++ ) {
		var child = children[i];
		if ( child.kind == 'category' ) html += html_category_tree(child.id, App.category);
	}
	html += "</div>";
	
	$("#category-edit-form-parent-tree").html(html);
	
	$("#category-edit-radio-" + cat[2]).click();
}

function category_tree_node(id)
{
	var cat;
	
	if ( id == 0 ) {
		cat = [0, cpp.jstr('Root')];
	} else {
		cat = cpp.category(id);
	}

	var html = "<div><label class='radio'><input id='category-tree-radio-" + cat[0] + "' type='radio' value='" + cat[0] + "' name='category-tree-radio'> " + cat[1] + "</label>";
	
	var children = JSON.parse(cpp.category_children(id));
	for (var i = 0; i < children.length; i++ ) {
		var child = children[i];
		if ( child.kind == 'category') html += category_tree_node(child.id);
	}
	
	html += "</div>";
	return html;
}

function question_add_answer()
{
	var html = '&nbsp;&nbsp;&nbsp;' +
	"<div class='input-prepend'>" +
	"<input class='add-on' type='radio' name='question-form-rigth-answer' />" +
	"<input type='text' name='question-form-answers-fields[]' class='input-xxlarge' />" +
	"</div>";

	$('#question-form-answers').append(html);
	    
    translate();
}

function test_form_categories(id)
{
    if ( id == undefined ) id = 0;
    
	var cat;
	
	if ( id == 0 ) {
		cat = [0, cpp.jstr('Root')];
	} else {
		cat = cpp.category(id);
	}
    
    var html = '<div>' + 
    '<label class="checkbox" for="test-category-checkbox-' + cat[0] + '">' +
    '<input data-id="' + cat[0] + '" id="test-category-checkbox-' + cat[0] + '" type="checkbox"> ' + cat[1] +
    '<a data-icon="check" data-text="" class="btn btn-link select-all-subitems"></a>' +
    '</label>';
    
    var children = JSON.parse(cpp.category_children(id));;
	for (var i = 0; i < children.length; i++ ) {
		var child = children[i];
		if ( child.kind == 'category') html += test_form_categories(child.id);
	}
    
    html += '</div>';
    
    return html;
};

function prepare_test_form()
{
    $('#test-form-title').val('');
    
    $('#test-form-header').val(cpp.last_header());
	$("#test-form-header").cleditor()[0].refresh();
	$("#test-form-header").cleditor()[0].updateFrame();
    
    $("#test-form-categories").html( test_form_categories() );
    
    $('#test-form-categories .select-all-subitems').click(function(e) {
        e.preventDefault();
        $(this).parent().parent().find('input').attr('checked',true);
    });
}

function question_edit_prepare()
{
	var id = $("#question-form-id").data('id');
	$("#question-form-category-tree").html(category_tree_node(0));
	if ( id < 0 ) {
		$("#question-form-title").val('');
		$('#question-form-difficulty').slider('value', 6);
		$("#question-form-reference").val('');
		$("#question-form-body").val('');
		
		$('#question-form-answers').html('');
		for (var i = 0; i < 4; i++){
			question_add_answer();
		}
		
		$("#category-tree-radio-" + App.category).click();
	} else {
		var q = cpp.question(id);
		$("#question-form-title").val(q[1]);
		$('#question-form-difficulty').slider('value', q[3]);
		$("#question-form-reference").val(q[4]);
		$("#question-form-body").val(q[2]);
		
		$('#question-form-answers').html('');
		
		var as = JSON.parse(cpp.answer_all(q[0]));
		for (var i = 0; i < as.length; i++) {
			question_add_answer();
			if ( as[i].right ) $('#question-form-answers div:last input[type=radio]').click();
			$('#question-form-answers div:last input[type=text]').val(as[i].body);
		}
		
		$("#category-tree-radio-" + q[5]).click();
	}

	$("#question-form-body").cleditor()[0].refresh();
	$("#question-form-body").cleditor()[0].updateFrame();
}

function test_add_question(id)
{
    var html = '<li class="print-question">';
    
    var question = cpp.question(id);
    
    html += '<h2 class="print-hide print-question-title">' + question[1] + '</h2>';
    html += '<div class="print-hide print-question-reference">' + question[4] + '</div>';
    html += '<div class="print-question-body">' + question[2] + '</div>';
    
    html += '<ol type="a" class="print-question-answers">';
    
	var as = JSON.parse(cpp.answer_all(question[0]));

    var r = Math.floor(Math.random()*51) + 50;
    for (var i=0; i<r; i++) as.sort(function() { return 0.5 - Math.random();});
    
    for (var i=0; i < as.length; i++) {
        var a = as[i];
        html += '<li';
        if (a.right) {
            App.test.answers.push(i);
            html += ' class="print-right-answer"';
        }
        html += '>' + a.body + '</li>';
    };
    
    html += '</ol></li>';
    
    return html;
}

function create_test()
{
    App.test = {};
    App.test.answers = [];

    var title = $('#test-form-title').val();
    var num_questions = $('#test-form-number-questions').val();
    var num_tests = $('#test-form-number-tests').val();
    var difficulty_min = $('#test-form-difficulty').slider('values',0);
    var difficulty_max = $('#test-form-difficulty').slider('values',1);
    var header = $('#test-form-header').val();
    var categories = [];
    
    // form validation
    if ( title == '' ) {
        $('#no-title-modal').reveal();
        return;
    }
    
    if ( isNaN(num_questions) || num_questions < 10 || num_questions > 50 ) {
        $('#wrong-number-of-questions').reveal();
        return;
    }
    
    if ( isNaN(num_tests) || num_tests < 1 ) {
        $('#wrong-number-of-tests').reveal();
        return;
    }
    
    $('#test-form-categories input:checked').each(function(i) {
        categories.push($(this).data('id'));
    });

    var questions = cpp.questions_for_print(categories, [difficulty_min, difficulty_max]);
    
    if( questions.length < num_questions ) {
        $('#not-enough-questions').reveal();
        return;
    }
    
    var html = '';
    // test creation
    
    for (var i=0; i < num_tests; i++) {
        // guarantees a better randomness
        var r = Math.floor(Math.random()*51) + 50;
        for (var j=0; j<r; j++) questions.sort(function() { return 0.5 - Math.random();});
        
        html += "<div class='print-header'>" + header + "</div>";

        html += '<ol>';
        for (var j=0; j < num_questions; j++) {
            html += test_add_question(questions[j]);
        }
        html += '</ol>';
        html += '<div class="page-break">&nbsp;</div>'
    }
    
    App.test = cpp.test_create(title, html, header);
    
    App.state = 'test-show';
    refreshUI();
}

function test_show_prepare()
{
    var test = cpp.test(App.test);
    
    $('#test-show-title').html(test[1]);
    $('#test-show-date').html(cpp.jstrdate(test[0]));
    $('#test-show-body').html(test[2]);
}