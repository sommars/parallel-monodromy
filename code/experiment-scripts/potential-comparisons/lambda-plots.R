library(latex2exp)

# fabricated

cvxFab <- read.csv("../../../experiments/1000fabricated.csv")
cvxFab2 <- read.csv("../../../experiments/1000fabricated2.csv")

summary(cvxFab)
summary(cvxFab2)

par(mfrow=c(1,2),oma=c(0,0,2,0))
plot(cvxFab$lambda,cvxFab$ntracks,xlab=TeX('$\\lambda$'),ylab="Average Number of Path Tracks")#,main=TeX('Random Solution Graph (d=1000,m=3,N=3,100 trials)'))
plot(cvxFab2$lambda,cvxFab2$ntracks,xlab=TeX('$\\lambda$'),ylab="Average Number of Path Tracks")#,main=TeX('Random Solution Graph (d=1000,m=3,N=3,100 trials)'))
title("Random Solution Graph (d=1000,m=3,N=3,100 trials)",outer=TRUE)
dev.copy(png, "../../../../parallel-monodromy-paper/paper/PIX/convex_fab_combined.png")
dev.off()

# real

cvx5 <- read.csv("cyclic-5.csv")
cvx7 <- read.csv("cyclic-7.csv")
#cvx9 <- read.csv("../../../experiments/9cyclic-5.csv")
#cvx8 <- read.csv("../../../experiments/8cyclic-5.csv")
#cfab_100<- read.csv("../../../experiments/fabricated.csv")
#cfab_90<- read.csv("../../../experiments/9fabricated.csv")
#cfab_80<- read.csv("../../../experiments/8fabricated.csv")

par(mfrow=c(1,2),oma=c(0,0,2,0))
plot(cvx5$lambda, cvx5$ntracks,xlab=TeX('$\\lambda$'),ylab="Average Number of Path Tracks",main=TeX('cyclic-5'))
plot(cvx7$lambda, cvx7$ntracks,xlab=TeX('$\\lambda$'),ylab="Average Number of Path Tracks",main=TeX('cyclic-7'))
#plot(cvx9$lambda, cvx9$ntracks,xlab=TeX('$\\lambda$'),ylab="Average Number of Path Tracks",main=TeX('Cyclic-5, $\\alpha = 0.9$'))
#plot(cvx8$lambda, cvx8$ntracks,xlab=TeX('$\\lambda$'),ylab="Average Number of Path Tracks",main=TeX('Cyclic-5, $\\alpha = 0.8$'))
#plot(cfab_100$lambda,cfab_100$ntracks,xlab=TeX('$\\lambda$'),ylab="Average Number of Path Tracks",main=TeX('Fabricated Data, $\\alpha = 1.0$'))
#plot(cfab_90$lambda,cfab_90$ntracks,xlab=TeX('$\\lambda$'),ylab="Average Number of Path Tracks",main=TeX('Fabricated Data, $\\alpha = 0.9$'))
#plot(cfab_80$lambda,cfab_80$ntracks,xlab=TeX('$\\lambda$'),ylab="Average Number of Path Tracks",main=TeX('Fabricated Data, $\\alpha = 0.8$'))
#title("Potential at Fixed Vertex <-> Potential at all Vertices",outer=TRUE)

dev.copy(png, "../../../paper/PIX/convex.png")
dev.off()
