(function() {
    var translation = {
        formatNumber: function(number) {
            return number;
        },
        
        'Totalizing %d questions': function(n) {
            return  'Totalizando ' + n + ' quest' + (n==1?'ão':'ões');
        },
        
        'Test %d': function(n) {
            return 'Teste ' + n;
        },
        
        '__lang__': 'Português Brasileiro',
        'Language': 'Idioma',
        'Root': 'Início'
    };
    
    AppI18N.registerTranslation('pt_BR', translation);
})();
