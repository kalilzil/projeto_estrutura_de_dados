d <- read.csv("comparacoes_n.csv")

plot(d$N, d$lista, type = "l", lwd = 2, col = "red",
     xlab = "Quantidade de dados (N)",
     ylab = "Média de comparações",
     main = "Lista encadeada vs Árvore de Busca Binária")

lines(d$N, d$abb, lwd = 2, col = "blue")

legend("topleft", legend = c("Lista", "ABB"),
       col = c("red", "blue"), lwd = 2, bty = "n")