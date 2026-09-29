# ADR: Aplicação c/ C, paralelismo e OpenMP usando dataset real

## Status
Aceito

## Data
2026-09-28

## Contexto
Como parte do trabalho 1 da matéria de Programação Paralela, esse programa utilizará a linguagem de programação C, com paralelismo usando a biblioteca OpenMP em uma máquina Windows com terminal Linux, por meio do WSL. O intuito é analisar um dataset real da bolsa de valores brasileira, cobrindo o período de 2010 a 2025 e contendo mais de um milhão de dados.

## Decisão
Decidimos utilizar a linguagem C para aproveitar os ensinamentos já passados em aula, em conjunto com a biblioteca OpenMP, devido às suas facilidades na hora de implementar, balancear e paralelizar processos. Por questões de simplicidade, será utilizada inicialmente apenas a branch "main" para agilizar a codificação, com novas branches sendo criadas caso o grupo veja necessidade.

## Consequências

**Positivo:**
* OpenMP nos possibilita uma implementação de paralelismo de uma forma bem mais facilitada em comparação com outras bibliotecas e com balanceamento automático.

**Negativo:**
* OpenMP é mais complexo para evitar problemas de condições de corrida no código;
* Paralelismo é mais complicado de implementar do que deixar tudo sequencial.

## Conformidade
* O repositório deverá ser público;
* A aplicação deverá ser devidamente documentada;
* Manter a etiqueta de *clean code*;
* Cada commit deverá seguir *conventional commits*;
* Caso seja criada mais uma branch, a mesma deverá ser nomeada também seguindo *conventional commits*;
* Ao término, o código precisa estar testado e funcional.

## Notas
* **Autores:** Henrique Luan Fritz, Luan Vitor Casali Dallabrida e Lucas Pannebecker Sckenal
* **Versão:** 0.1
* **Changelog:**
  * 0.1: Versão inicialmente proposta

---

## Preparação dos Dados
O dataset utilizado (`bovespa_stocks.csv`) está compactado no arquivo `dataset.zip`. Certifique-se de extrair o CSV para a pasta raiz do projeto antes de compilar e executar o código.

## Como Executar
Para rodar a aplicação via terminal Linux (WSL), utilize os comandos de compilação abaixo:

```bash
# Compilar e executar a versão sequencial
gcc -o sequencial app_sequencial.c -fopenmp
./sequencial

# Compilar e executar a versão paralela
gcc -o paralelo app_paralelo.c -fopenmp
./paralelo
```