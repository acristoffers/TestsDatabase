(function() {
    var translation = {
        formatNumber: function(number) {
            return number;
        },
        
        'Test %d': function(n) {
            return 'Prüfung ' + n;
        },
        
        'There is not %d questions with difficulty between %d and %d on the selected categories': function(n) {
            return  'Es gibt nicht ' + n[0] + ' Frage' + (n[0]==1?'':'n') + ' mit Schwierigkeiten zwischen ' + n[1] + ' und ' + n[2] + ' in den ausgewählten Kategorien.'
        },
        
        'Totalizing %d questions': function(n) {
            return  'Insgesamt ' + n + ' Frage' + (n==1?'':'n');
        },
        
        'Version %d': function(n) {
            return 'Version ' + n;
        },
        
        '__lang__': 'Deutsch',
        
        '© Álan Crístoffer. All rights reserved.': '© Álan Crístoffer. Alle Rechte vorbehalten.',
        'About': 'Über',
        'Add category': 'Kategorie hinzufügen',
        'Add difficulty': 'Schwierigkeit hinzufügen',
        'Add Question': 'Frage hinzufügen',
        'Answers:': 'Antworten:',
        'Cancel': 'abbrechen',
        'Categories': 'Kategorien',
        'Categories/Questions': 'Kategorien/Fragen',
        'Create Test': 'Prüfung hinzufügen',
        'Delete': 'löschen', 
        'Difficulty:': 'Schwierigkeit:',
        'Difficulty from': 'Schwierigkeit von', 
        'Edit': 'bearbeiten',
        'Edit Category': 'Kategorie bearbeiten',
        'Edit Question': 'Frage bearbeiten',
        'File': 'Ablage',
        'Generate': 'erzeugen', 
        'Language': 'Sprache',
        'Merge': 'verschmelzen', 
        'Name:': 'Name:', 
        'New': 'Neue',
        'New Question': 'Neue Frage',
        'New Test': 'Neue Prüfung',
        'Not enough questions.': 'Es gibt nicht genug Fragen.',
        'No name...': 'Kein Name...', 
        'No right answer': 'Keine richtige Antwort', 
        'No title...': 'Kein Titel...', 
        'Number of tests': 'Anzahl der Prüfung',
        'Number of questions': 'Anzahl der Antwort',
        'Open': 'Öffnen',
        'Page header': 'Kopfzeile', 
        'Print Answers': 'Drucken Antworten',
        'Print Answers Sheet': 'Druckvorlage',
        'Print Tests': 'Drucken Prüfungen',
        'Questions': 'Fragen',
        'Reference:': 'Referenz:',
        'Remove': 'löschen',
        'Root': 'Einleitung',
        'Save': 'Sparen',
        'Show': 'Anzeigen',
        'Tests': 'Prüfungen',
        'Tests Database': 'Fragen Bank',
        'Title:': 'Titel:',
        'to':  'nach',
        'You forgot to type the name...': 'Sie haben vergessen, den Namen eingeben...',
        'You forgot to type the title...': 'Sie haben vergessen, den Titel eingeben...',
        'You forgot to select the right answer...': 'Sie haben vergessen, die richtige Antwort zu wählen...'
    };
    
    AppI18N.registerTranslation('de', translation);
})();
