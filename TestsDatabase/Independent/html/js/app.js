/* (c) 2012 Álan Crístoffer */

var App = {
    categoryDelete: function() {
        var id = AppNav.current.category;
        var parent = AppCore.categorySelect(AppNav.current.category).parent;
        
        AppUI.showModalAlert({
            title: 'Are you sure?',
            content: 'Are you sure you want to delete this category?\nThis operation can\'t be undone.',
            button: {
                class: 'danger',
                text: 'Delete'
            }
        }, function() {
            AppCore.categoryDelete(id);
            
            AppNav.current.category = parent;
            AppNav.navigate('#/category/show/' + parent);
        });
    },
    
    categoryInsert: function() {
        var name = $('#category-new-modal-title').val();
        $('#category-new-modal-title').val('');
        
        AppCore.categoryInsert(name, AppNav.current.category);
        
        AppUI.closeModals();
        
        AppNav.navigate('#/category/show/' + AppNav.current.category);
    },
    
    categoryMerge: function() {
        AppUI.showModalAlert({
            title: 'Are you sure?',
            content: 'Are you sure you want to merge this category?\nThis operation can\'t be undone.',
            button: {
                class: 'danger',
                text: 'Merge'
            }
        }, function() {
            var id = AppNav.current.category;
            
            var np = $("input[name=category-tree-radio]:checked").val();
            
            var subcats = AppCore.listCategories(id);
            for (var i = subcats.length - 1; i >= 0; i--) {
                var cat = subcats[i];
                AppCore.categoryUpdate(cat.id, cat.name, np);
            }
            
            var subqtns = AppCore.listQuestions(id);
            for (var i = subqtns.length - 1; i >= 0; i--){
                var q = subqtns[i];
                AppCore.questionUpdate(q.id, q.title, q.reference, q.difficulty, q.body, np);
            }
            
            AppCore.categoryDelete(id);
            
            AppNav.navigate('#/category/show/' + np);
        });
    },
    
    categoryUpdate: function() {
        var id = AppNav.current.category;
        var name = $('#category-edit-form-name').val();
        var parent = $("input[name=category-tree-radio]:checked").val();
        
        if ( name == '' ) {
            AppUI.showModalAlert({
                title: 'No name...',
                content: 'You forgot to type the name...',
                cancel: false,
                button: {
                    text: 'Ok'
                }
            }, AppUI.closeModals);
            return;
        }
        
        AppCore.categoryUpdate(id, name, parent);
        
        AppNav.navigate('#/category/show/'+id);
    },
    
    databaseChanged: function() {
        if ( AppCore.checkDatabase() )
            AppNav.navigate('#/category/show/0');
        else
            AppNav.navigate('#/database/showdialog');
    },
    
    populateDatabase: function(id, i) {
        if ( id == undefined )
            id = 0;
            
        if ( i == undefined )
            i = 1;
        
        if ( i > 3)
            return;
        
        id = AppCore.categoryInsert('Cat on ' + id, id);
        for (var f=0; f < 3; f++)
            App.populateDatabase(id, i+1);
				
		var text = 'Wurden früher die wenigen Zeilen von „Lorem ipsum“ im Blindtext fortlaufend wiederholt, so dient heute der aufgefundene Text von Cicero als Basis vieler Lorem-ipsum-Generatoren, die darauf aufbauend längere Abschnitte erzeugen. Die Begriffsfolge ist so weithin üblich, dass viele Desktop-Publishing-Programme einen Menüpunkt für Blindtext haben, der eine Sequenz erzeugt, die mit „Lorem ipsum“ beginnt. Außerdem wird heute die Begriffsfolge „Lorem ipsum“ in der elektronischen Druckaufbereitung erkannt und eine Warnmeldung ausgegeben, damit eine Publikation mit verbliebenem Blindtext nicht versehentlich in den Druck geht. Auch für den Textsatz mit LaTeX gibt es Ergänzungspakete.';
            
        for ( var f=0; f<100; f++ ) {
            var dif = Math.floor(Math.random()*11);
            if ( dif == 0 ) dif = 10;
            var q = AppCore.questionInsert('Question dif=' + dif + ' on ' + id, 'ref', dif, text.substr(0, Math.floor(Math.random()*text.length)), id, 0);
            AppCore.answerInsert('right ' + text.substr(0, Math.floor(Math.random()*text.length)), q, true);
            AppCore.answerInsert('wrong ' + text.substr(0, Math.floor(Math.random()*text.length)), q, false);
            AppCore.answerInsert('wrong ' + text.substr(0, Math.floor(Math.random()*text.length)), q, false);
            AppCore.answerInsert('wrong ' + text.substr(0, Math.floor(Math.random()*text.length)), q, false);
        }
        
        AppNav.navigate('#/category/show/0');
    },
    
    printTests: function() {
        $('#tests-wrapper').css('font-size', $('#print-font-size').val() + 'pt');
		$('#tests-wrapper *').css('line-height', ( parseInt($('#print-font-size').val()) * 1.2 ) + 'pt');
		
		$('.test').addClass('dont-print');
		$($('.test')[$('#print-test-num').val()-1]).removeClass('dont-print');
		
        $('#tests-wrapper').removeClass('dont-print');
        $('#test-answers-sheets').addClass('dont-print');
        $('#clean-answers-sheets').addClass('dont-print');
		
		print();
		
        $('#tests-wrapper').css('font-size', 'inherit');
		$('#tests-wrapper *').css('line-height', 'inherit');
    },
    
    printAnswers: function() {
        $('#tests-wrapper').addClass('dont-print');
        $('#test-answers-sheets').removeClass('dont-print');
        $('#clean-answers-sheets').addClass('dont-print');
        print();
    },
    
    printAnswerSheets: function() {
        $('#tests-wrapper').addClass('dont-print');
        $('#test-answers-sheets').addClass('dont-print');
        $('#clean-answers-sheets').removeClass('dont-print');
        print();
    },
    
    questionDelete: function() {
        var id = AppNav.current.question;
        
        AppUI.showModalAlert({
            title: 'Are you sure?',
            content: 'Are you sure you want to delete this question?\nThis operation can\'t be undone.',
            button: {
                class: 'danger',
                text: 'Delete'
            }
        }, function() {
            AppCore.questionDelete(id);
            AppNav.navigate('#/category/show/' + AppNav.current.category);
        });
    },
    
    modelChanged: function() {
        App.databaseChanged();
    },
    
    questionInsert: function() {
		var id = AppNav.current.question;
		var title = $("#question-form-title").val();
		var difficulty = $('#question-form-difficulty').slider('value');
		var reference = $("#question-form-reference").val();
		var body = $("#question-form-body").val();
		var category = $('input[name=category-tree-radio]:checked').val();
        
        if ( title == '' ) {
            AppUI.showModalAlert({
                title: 'No title...',
                content: 'You forgot to type the title...',
                cancel: false,
                button: {
                    text: 'Ok'
                }
            }, AppUI.closeModals);
            return;
        }
		
		var answers = [];
		var right_answer = null;
		
		if ( !$('#question-form-kind').is(':checked') ) {
			$('input[name^=question-form-answers-fields]').each(function() {
				var r = $(this).prev('input').is(':checked');
				if ( r ) {
					right_answer = $(this).val();
				} else {
					answers.push($(this).val());
				}
			});
			
			if ( right_answer == null ) {
	            AppUI.showModalAlert({
	                title: 'No right answer',
	                content: 'You forgot to select the right answer...',
	                cancel: false,
	                button: {
	                    text: 'Ok'
	                }
	            }, AppUI.closeModals);
				return;
			}
		}
		
		var kind = $('#question-form-kind').is(':checked');
		
		if ( id < 0 ) {
			id = AppCore.questionInsert(title, reference, difficulty, body, category, kind);
		} else {
			AppCore.questionUpdate(id, title, reference, difficulty, body, category, kind);
			AppCore.answerDelete(id);
		}
        
		if ( !$('#question-form-kind').is(':checked') ) {
			AppCore.answerInsert(right_answer, id, true);
			for (var i = 0; i < answers.length; i++ ) {
				AppCore.answerInsert(answers[i], id, false);
			}
		}
		
		AppNav.current.category = category;
        AppNav.navigate('#/question/show/'+id);
    },
    
    testDelete: function() {
        AppUI.showModalAlert({
            title: 'Delete test',
            content: 'Are you sure you want to delete this test?',
            button: {
                class: 'danger',
                text: 'Delete'
            }
        }, function() {
            AppCore.testDelete(AppNav.current.test);
            AppNav.navigate('#/test/list'); 
        });
    },
    
    testHandInsert: function() {
        var title = $('#test-create-title').val();
        var numOfTests = $('#test-create-number-tests').val();
        
        var header = $('#test-create-header').val();
        
        var q = $('#test-create-questions input:checked');
        var numberOfQuestions = q.length;
        
        var selectedQuestions = [];
		var discursiveQuestions = [];
        for (var i = q.length - 1; i >= 0; i--) {
			var sq = AppCore.questionSelect( q[i].value );
			if ( sq.kind == 0 )
				selectedQuestions.push( sq.id );
			else
				discursiveQuestions.push( sq.id );
        }
        
        if ( title == '' ) {
            AppUI.showModalAlert({
                title: 'No title...',
                content: 'You forgot to type the title...',
                cancel: false,
                button: {
                    text: 'Ok'
                }
            }, function() {
                AppUI.closeModals();
                $('#test-form-title').focus();
            });
            return;
        }
        
        if ( numberOfQuestions < 0 || numberOfQuestions > 50 ) {
            AppUI.showModalAlert({
                title: 'Invalid number of questions',
                content: 'Did you forget to select questions and difficulties?',
                cancel: false,
                button: {
                    text: 'Ok'
                }
            }, AppUI.closeModals);
            return;
        }
        
        function randomPool(i, n)
        {
            var pool = [];
            
            for(var j=0; j<i; j++)
                pool.push(Math.floor(Math.random()*n));
            
            pool = pool.filter(function(elem, pos) {
                return pool.indexOf(elem) == pos;
            });
            
            while ( pool.length < i ) {
                pool.push(Math.floor(Math.random()*n));
                
                pool = pool.filter(function(elem, pos) {
                    return pool.indexOf(elem) == pos;
                });
            }
            
            return pool;
        }
        
        var tests = '<div id="tests-wrapper">';
        
        var answerSheets = '';
        
        for (var i=0; i < numOfTests; i++) {
            var test = '<div class="test">';
            test += '<div class="test-header">' + header + '</div>';
            test += '<div class="test-number" data-i18n="Test %d" data-i18n-numbers="' + (i+1) + '"></div>';
            test += '<div class="test-questions">';
            
            var as   = '<div class="answers-sheet"><div id="answers-header">' + header + '</div><div class="test-number" data-i18n="Test %d" data-i18n-numbers="' + (i+1) + '"></div><div class="answers"><div class="acol">';
            
            var qp = randomPool(selectedQuestions.length, selectedQuestions.length);
			var question_number = 0;
            for (var j = 0; j < selectedQuestions.length; j++) {
				question_number = j+1;
                var q = AppCore.questionSelect(selectedQuestions[qp[j]]);
                
                var question = '<div class="test-question"><span class="bullet">' + (j+1) + '. </span> <span class="question-body">' +
                               '<div class="dont-print question-meta">';
                
                question +=  AppI18N.tr('<span data-i18n="Title:"></span> ') + q.title +
                             AppI18N.tr('<br><span data-i18n="Reference:"></span> ') + q.reference +
                             AppI18N.tr('<br><span data-i18n="Difficulty:"></span> ') + q.difficulty;
                
                question += '</div>' + q.body;
                
                question += '<div class="question-answers">';
                
                if ( j == 10 || j == 20 || j == 30 || j == 40 )
                    as += '</div><div class="acol">';
                
                as += '<div class="arow"><div class="question-number">' + (j+1) + '</div>';
                
                var answers = AppCore.answerSelect(q.id);
                var ap = randomPool(answers.length, answers.length);
                var aa = ['a', 'b', 'c', 'd'];
                for (var k = 0; k < answers.length; k++) {
                    question += '<div class="question-alternative"><span class="bullet">' + aa[k] + ') </span> <span> ' + answers[ap[k]].body + '</span></div>';
                    as += '<div class="alternative ' + (parseInt(answers[ap[k]].right) == 1 ?  'right' :  '') + '">' + aa[k] + '</div>';
                }
                
                as += '</div>';
                
                question += '</div></span></div>';
                
                test += question;
            }
			
			for (var j = 0; j < discursiveQuestions.length; j++) {
                var q = AppCore.questionSelect( discursiveQuestions[j] );
                
                var question = '<div class="test-question"><span class="bullet">' + (question_number+1) + '. </span> <span class="question-body">' +
                               '<div class="dont-print question-meta">';
                
                question +=  AppI18N.tr('<span data-i18n="Title:"></span> ') + q.title +
                             AppI18N.tr('<br><span data-i18n="Reference:"></span> ') + q.reference +
                             AppI18N.tr('<br><span data-i18n="Difficulty:"></span> ') + q.difficulty;
                
                question += '</div>' + q.body;
				question += '<hr><br> <hr><br> <hr><br> <hr><br> <hr><br> <hr><br>';
				question += '</span></div>';
				
				test += question;
				question_number++;
			}
            
            for (var j = selectedQuestions.length; j < 50; j++ ) {
                if ( j == 10 || j == 20 || j == 30 || j == 40 )
                    as += '</div><div class="acol">';
                
                as += '<div class="arow"><div class="question-number">' + (j+1) + '</div>';
                    
                var aa = ['a', 'b', 'c', 'd'];
                for (var k = 0; k < 4; k++)
                    as += '<div class="alternative">' + aa[k] + '</div>';
                    
                as += '</div>';
            }
            
            as += '</div></div></div>';
            
            test += '</div>';
            test += '</div>';
            
            tests += test;
            
            answerSheets += as;
        }
        
        tests += '</div>';
        
        tests += '<div id="test-answers-sheets">' + answerSheets + '</div>';
        
        tests += '<div id="clean-answers-sheets">';
        for ( var i = 0; i < numOfTests; i++) {
            tests += '<div class="answers-sheet"><div id="answers-header">' + header + '</div><div class="test-number" data-i18n="Test %d" data-i18n-numbers="' + (i+1) + '"></div><div class="answers"><div class="acol">';
            
            for (var j = 0; j < 50; j++ ) {
                if ( j == 10 || j == 20 || j == 30 || j == 40 )
                    tests += '</div><div class="acol">';
                
                tests += '<div class="arow"><div class="question-number">' + (j+1) + '</div>';
                
                var aa = ['a', 'b', 'c', 'd'];
                for (var k = 0; k < 4; k++)
                    tests += '<div class="alternative">' + aa[k] + '</div>';
                
                tests += '</div>';
            }
            tests += '</div></div></div>';
        }
        
        tests += '</div>';
        
        var id = AppCore.testInsert(title, tests, header);
        AppNav.navigate('#/test/show/' + id);
    },
    
    testInsert: function() {
        var title = $('#test-form-title').val();
        var numOfTests = $('#test-form-number-tests').val();
        
        var numberOfQuestions = 0;
        var difficulties = [];
        $('#test-form-question-difficulties .difficulty-setup').each(function() {
            var n = $( $(this).children('.questions')[0]  ).val();
            var f = $( $(this).children('.diff-from')[0]  ).val();
            var t = $( $(this).children('.diff-to')[0]    ).val();
			var d = $( $(this).children('.discursive')[0] ).is(':checked');
			
            difficulties.push( {
                questions: n,
                from: f,
                to: t,
				kind: (d == true ? 1 : 0)
            });
            
            numberOfQuestions += parseInt(n);
        });
        
        var header = $('#test-form-header').val();
        
        var categories = [];
        $('#test-form-categories input:checked').each(function(i) {
            categories.push($(this).data('id'));
        });
        
        if ( title == '' ) {
            AppUI.showModalAlert({
                title: 'No title...',
                content: 'You forgot to type the title...',
                cancel: false,
                button: {
                    text: 'Ok'
                }
            }, function() {
                AppUI.closeModals();
                $('#test-form-title').focus();
            });
            return;
        }
        
        if ( numberOfQuestions < 0 || numberOfQuestions > 50 ) {
            AppUI.showModalAlert({
                title: 'Invalid number of questions',
                content: 'Did you forget to select questions and difficulties?',
                cancel: false,
                button: {
                    text: 'Ok'
                }
            }, AppUI.closeModals);
            return;
        }
        
        // generates a pool of random, unique numbers
        function randomPool(i, n)
        {
            var pool = [];
            
            for(var j=0; j<i; j++) {
                pool.push(Math.floor(Math.random()*n));
            }
            
            pool = pool.filter(function(elem, pos) {
                return pool.indexOf(elem) == pos;
            });
            
            while ( pool.length < i ) {
                pool.push(Math.floor(Math.random()*n));
                
                pool = pool.filter(function(elem, pos) {
                    return pool.indexOf(elem) == pos;
                });
            }
            
            return pool;
        }
        
        var selectedQuestions = [];
		var discursiveQuestions = [];
        
        for (var i = difficulties.length - 1; i >= 0; i--) {
            var d = difficulties[i];
			
            var questions = AppCore.questionSelectIds(categories, d.from, d.to, d.kind);
            if ( questions.length < d.questions ) {
                AppUI.showModalAlert({
                    title: 'Not enough questions.',
                    content: AppI18N.tr('There is not %d questions with difficulty between %d and %d on the selected categories', [d.questions, d.from, d.to]),
                    cancel: false,
                    button: {
                        text: 'Ok'
                    }
                }, AppUI.closeModals);
                return;
            }
            
            var rp = randomPool(d.questions, questions.length);
			
            for (var j=0; j < d.questions; j++)
				if ( d.kind == 0 )
                	selectedQuestions.push( questions[rp[j]] );
				else
					discursiveQuestions.push( questions[rp[j]] );
        }
        
        var tests = '<div id="tests-wrapper">';
        
        var answerSheets = '';
        
        for (var i=0; i < numOfTests; i++) {
            var test = '<div class="test">';
            test += '<div class="test-header">' + header + '</div>';
            test += '<div class="test-number" data-i18n="Test %d" data-i18n-numbers="' + (i+1) + '"></div>';
            test += '<div class="test-questions">';
            
            var as   = '<div class="answers-sheet"><div id="answers-header">' + header + '</div><div class="test-number" data-i18n="Test %d" data-i18n-numbers="' + (i+1) + '"></div><div class="answers"><div class="acol">';
            
            var qp = randomPool(selectedQuestions.length, selectedQuestions.length);
			var question_number = 0;
            for (var j = 0; j < selectedQuestions.length; j++) {
				question_number = j+1;
                var q = AppCore.questionSelect(selectedQuestions[qp[j]]);
                
                var question = '<div class="test-question"><span class="bullet">' + (j+1) + '. </span> <span class="question-body">' +
                               '<div class="dont-print question-meta">';
                
                question +=  AppI18N.tr('<span data-i18n="Title:"></span> ') + q.title +
                             AppI18N.tr('<br><span data-i18n="Reference:"></span> ') + q.reference +
                             AppI18N.tr('<br><span data-i18n="Difficulty:"></span> ') + q.difficulty;
                
                question += '</div>' + q.body;
                
                question += '<div class="question-answers">';
                
                if ( j == 10 || j == 20 || j == 30 || j == 40 )
                    as += '</div><div class="acol">';
                
                as += '<div class="arow"><div class="question-number">' + (j+1) + '</div>';
                
                var answers = AppCore.answerSelect(q.id);
                var ap = randomPool(answers.length, answers.length);
                var aa = ['a', 'b', 'c', 'd'];
                for (var k = 0; k < answers.length; k++) {
                    question += '<div class="question-alternative"><span class="bullet">' + aa[k] + ') </span> <span> ' + answers[ap[k]].body + '</span></div>';
                    as += '<div class="alternative ' + (parseInt(answers[ap[k]].right) == 1 ?  'right' :  '') + '">' + aa[k] + '</div>';
                }
                
                as += '</div>';
                
                question += '</div></span></div>';
                
                test += question;
            }
			
			for (var j = 0; j < discursiveQuestions.length; j++) {
                var q = AppCore.questionSelect( discursiveQuestions[j] );
                
                var question = '<div class="test-question"><span class="bullet">' + (question_number+1) + '. </span> <span class="question-body">' +
                               '<div class="dont-print question-meta">';
                
                question +=  AppI18N.tr('<span data-i18n="Title:"></span> ') + q.title +
                             AppI18N.tr('<br><span data-i18n="Reference:"></span> ') + q.reference +
                             AppI18N.tr('<br><span data-i18n="Difficulty:"></span> ') + q.difficulty;
                
                question += '</div>' + q.body;
				question += '<hr><br> <hr><br> <hr><br> <hr><br> <hr><br> <hr><br>';
				question += '</span></div>';
				
				test += question;
				question_number++;
			}
            
            for (var j = selectedQuestions.length; j < 50; j++ ) {
                if ( j == 10 || j == 20 || j == 30 || j == 40 )
                    as += '</div><div class="acol">';
                
                as += '<div class="arow"><div class="question-number">' + (j+1) + '</div>';
                
                var aa = ['a', 'b', 'c', 'd'];
                for (var k = 0; k < 4; k++)
                    as += '<div class="alternative">' + aa[k] + '</div>';
                
                as += '</div>';
            }
            
            as += '</div></div></div>';
            
            test += '</div>';
            test += '</div>';
            
            tests += test;
            
            answerSheets += as;
        }
        
        tests += '</div>';
        
        tests += '<div id="test-answers-sheets">' + answerSheets + '</div>';
        
        tests += '<div id="clean-answers-sheets">';
        for ( var i = 0; i < numOfTests; i++) {
            tests += '<div class="answers-sheet"><div id="answers-header">' + header + '</div><div class="test-number" data-i18n="Test %d" data-i18n-numbers="' + (i+1) + '"></div><div class="answers"><div class="acol">';
            
            for (var j = 0; j < 50; j++ ) {
                if ( j == 10 || j == 20 || j == 30 || j == 40 )
                    tests += '</div><div class="acol">';
                
                tests += '<div class="arow"><div class="question-number">' + (j+1) + '</div>';
                
                var aa = ['a', 'b', 'c', 'd'];
                for (var k = 0; k < 4; k++)
                    tests += '<div class="alternative">' + aa[k] + '</div>';
                
                tests += '</div>';
            }
            tests += '</div></div></div>';
        }
        
        tests += '</div>';
        
        var id = AppCore.testInsert(title, tests, header);
        AppNav.navigate('#/test/show/' + id);
    }
};

$(document).ready(function() {
    AppUI.clean();
    AppUI.listAvailableTranslations();
    AppI18N.translate();
    
	App.databaseChanged();
});
