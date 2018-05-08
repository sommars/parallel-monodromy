library(latex2exp)

# fabricated

cvxFab100 <- read.csv("70fabricated.csv")
cvxFab1000 <- read.csv("924fabricated.csv")
cvxFab10000 <- read.csv("11016fabricated.csv")
cvx5 <- read.csv("cyclic-5.csv")
cvx7 <- read.csv("cyclic-7.csv")
cvx9 <- read.csv("cyclic-9.csv")

summary(cvxFab100)

par(mfrow=c(3,2),oma=c(0,0,2,0))
plot(cvxFab100$lambda,cvxFab100$ntracks,xlab=TeX('$\\lambda$'),ylab="# tracks")
plot(cvx5$lambda, cvx5$ntracks,xlab=TeX('$\\lambda$'),
     ylab="",
     main=TeX('cyclic-5'))
plot(cvxFab1000$lambda,cvxFab1000$ntracks,xlab=TeX('$\\lambda$'),ylab="# tracks")
plot(cvx7$lambda, cvx7$ntracks,xlab=TeX('$\\lambda$'),
     ylab="",
     main=TeX('cyclic-7'))
plot(cvxFab10000$lambda,cvxFab10000$ntracks,xlab=TeX('$\\lambda$'),ylab="# tracks")
plot(cvx9$lambda, cvx9$ntracks,xlab=TeX('$\\lambda$'),
     ylab="",
     main=TeX('cyclic-9'))

dev.copy(png, "../../../../parallel-monodromy-paper/paper/PIX/lambda_combined.png")
dev.off()

