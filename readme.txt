Projeto de Sockets da disciplina de rede de computadores, desenvolvido pelos alunos Felipe S. Andrade, Gustavo K. Fukuda e Pedro L. Olivo.

Como utilizar o programa:

Necessário o uso do gcc para compilar os arquivos.
Os dois principais executáveis são o servidor e cliente.
No terminal utilize: 'gcc -o servidor servidor.c Practical.c' e 'gcc -o cliente cliente.c Practical.c' para compila-los.
Apartir disso temos algumas maneiras de se executar o programa:
No terminal vá para o diretório dos arquivos (No nosso caso ele chamasse socket) e execute o comando './servidor 8080' 
(A porta não necessáriamente precisa ser essa, porém recomenda-se por conta dos testes feitos).
Abra outro terminal e vá novamente para o diretório dos arquivos e execute 'curl http://localhost:8080/' ou './cliente 127.0.0.1 /index.html 8080'.
Ambos fazem requisições HTTP GET para um servidor web local na porta 8080, sendo curl, um cliente HTTP genérico do sistema e 
./cliente seu próprio programa cliente.
Também pode-se executar via navegador web, apenas utilize o link 'http://localhost:8080/' na barra de pesquisa após excutar './servidor 8080'.