(function() {
    var translation = {
        formatNumber: function(number) {
            return number;
        },
        
        'Test %d': function(n) {
            return 'Prova ' + n;
        },
        
        'There is not %d questions with difficulty between %d and %d on the selected categories': function(n) {
            return 'Não há ' + n[0] + (n[0]==1?' questão':' questões') + ' com dificuldade entre ' + n[1] + ' e ' + n[2] + ' nas categorias selecionadas';
        },
        
        'Totalizing %d questions': function(n) {
            return  'Totalizando ' + n + ' quest' + (n==1?'ão':'ões');
        },
        
        'Version %d': function(n) {
            return 'Versão ' + n;
        },
        
        '__lang__': 'Português Brasileiro',
        
        '© Álan Crístoffer. All rights reserved.': '© Álan Crístoffer. Todos os direitos reservados.',
        'About': 'Sobre',
        'Add category': 'Adicionar categoria',
        'Add difficulty': 'Adicionar dificuldade',
        'Add Question': 'Adicionar questão',
        'Answers:': 'Respostas:',
        'Cancel': 'Cancelar',
        'Categories': 'Categorias',
        'Categories/Questions': 'Categorias/Questões',
        'Create Test': 'Criar Prova',
        'Delete': 'Excluir', 
        'Difficulty:': 'Dificuldade:',
        'Difficulty from': 'Dificuldade de', 
        'Edit': 'Editar',
        'Edit Category': 'Editar Categoria',
        'Edit Question': 'Editar Questão',
        'File': 'Arquivo',
        'Font size:': 'Tamanho da fonte:',
        'Generate': 'Gerar', 
        'Language': 'Idioma',
        'Merge': 'Mesclar', 
        'Name:': 'Nome:', 
        'New': 'Novo',
        'New Question': 'Nova Questão',
        'New Test': 'Nova Prova',
        'Not enough questions.': 'Não há questões o suficiente.',
        'No name...': 'Nenhum nome...', 
        'No right answer': 'Nenhuma resposta certa', 
        'No title...': 'Nenhum título...', 
        'Number of tests': 'Número de provas',
        'Number of questions': 'Número de questões',
        'Open': 'Abrir',
        'Page header': 'Cabeçalho', 
        'Print Answers': 'Imprimir Respostas',
        'Print Answers Sheet': 'Imprimir Gabaritos',
		'Print test number ': 'Imprir prova número',
        'Print Tests': 'Imprimir Provas',
        'Questions': 'Questões',
        'Reference:': 'Referência:',
        'Remove': 'Excluir',
        'Root': 'Início',
        'Save': 'Salvar',
        'Show': 'Exibir',
        'Tests': 'Provas',
        'Tests Database': 'Banco de Questões',
        'Title:': 'Título:',
        'to':  'até',
        'You forgot to type the name...': 'Você se esqueceu de digitar o nome...',
        'You forgot to type the title...': 'Você se esqueceu de digitar o título...',
        'You forgot to select the right answer...': 'Você se esqueceu de selecionar a resposta certa...'
    };
    
    AppI18N.registerTranslation('pt_BR', translation);
})();
