# 🌊🚢 BATALHA NAVAL 🚢🌊

<img src=src/Imagem_divulgação.PNG alt="Imagem divulgação" width="25%">

> Um jogo de Batalha Naval completo com interface colorida, efeitos visuais e habilidades especiais!

## ✨ Recursos Incríveis

- 🎨 **Interface colorida** com cores vibrantes
- 🚢 **Sistema de navios** em 4 direções diferentes
- 💥 **Habilidades especiais** com efeitos visuais
- 🔥 **Animações de impacto** quando acerta um navio
- 📊 **Tabuleiro ampliado** para melhor visualização
- 📱 **Menu interativo** fácil de usar

## 🕹️ Como Jogar

1. **Posicione seus navios** (horizontal, vertical ou diagonal)
2. **Use habilidades especiais** para vantagem tática
3. **Ataque coordenadas** para destruir navios inimigos
4. **Destrua todos os navios** para vencer!

## 🛠️ Tecnologias Utilizadas

```python
- Linguagem: C puro
- Bibliotecas: stdio.h, stdbool.h, windows.h
- Cores: ANSI Escape Codes (256 cores)
- Efeitos: ASCII Art e animações simples
```

## 🚀 Como Executar

Clone o repositório:

```bash
git clone https://github.com/WillianCiurlleti/Batalha_naval.git
```

Acesse o diretório:

```bash
cd Batalha_naval
```

### Windows
```bash
gcc batalha_naval.c -o batalha_naval
./batalha_naval.exe
```

### Linux/Mac
```bash
gcc batalha_naval.c -o batalha_naval
./batalha_naval
```

> ⚠️ Certifique-se de ter o GCC instalado!

## 🎮 Controles

| Tecla | Ação                |
|-------|---------------------|
| 1-4   | Menu de navios      |
| 1-3   | Menu de habilidades |
| 5     | Voltar              |
| Enter | Confirmar           |

## 📸 Capturas de Tela

| Menu Principal | Posicionar Navios | Habilidades |
|----------------|-----------|-------------|
| ![Menu principal](/src/Menu_principal.png) | ![Posicionar Navios](/src/Posicionar_navios.png) | ![Habilidades](/src/Aplicar_habilidades.png) |


| Atirar | Visualizar Tabuleiro | Visualizar Tabuleiro com navios |
|----------------|-----------|-------------|
| ![Atirar](/src/Tela_atirar.png) | ![Visualizar Tabuleiro](/src/Visualizar_tabuleiro.png) | ![Visualizar Tabuleiro com navios](/src/Visualizar_tabuleiro_com_navios.png) |

## 🧩 Estrutura do Código

```c
batalha_naval.c
├── Variáveis Globais
├── Inicialização
├── Renderização
│   ├── Tabuleiro
│   ├── Menus
│   └── Efeitos Visuais
├── Lógica do Jogo
│   ├── Posicionamento
│   ├── Habilidades
│   └── Sistema de Ataque
└── Fluxo Principal
```

## 🌈 Esquema de Cores

| Elemento       | Cor           | Exemplo       |
|----------------|---------------|---------------|
| Água           | Azul marinho  | `BG_BLUE`     |
| Navios         | Verde         | `BG_GREEN`    |
| Acertos        | Vermelho      | `BG_RED`      |
| Habilidades    | Roxo          | `BG_PURPLE`   |
| Texto          | Dourado       | `TXT_GOLD`    |


## 📦 Arquivo  Executável

- [Batallha_naval.exe](Batallha_naval.exe)



## ✉️ Contato

**Desenvolvedor:** Willian César  Ciurlleti    
🔗 LinkedIn: [linkedin.com/in/willian-ciurlleti](https://www.linkedin.com/in/willian-ciurlleti/)



<div align="center">
  <p>Desenvolvido com ❤️ usando C</p>
  <img src="https://img.shields.io/github/stars/WillianCiurlleti/Batalha_naval?style=social" alt="Stars">
</div>