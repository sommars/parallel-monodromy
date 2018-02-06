library(latex2exp)

# plot(table(rnbinom(10000,size=10,prob=0.5)))

rootCount<-1000
nodeCount<-3
edgeCounts<-c(2,3,5)    

createPlot = function (e,legsize,legend=FALSE,predict=FALSE,tracks=TRUE) {
    df <- read.csv(paste("../../../experiments/", e ,"edges.csv",sep=""))
    df$suc <- as.factor(df$suc)
    df$tottime <- as.numeric(df$tottime)
    df$ntracks <- as.numeric(df$ntracks)
    colors = c("red","blue","green")[df$suc]
    x = df$tottime
    if (tracks == TRUE) {
        x = df$ntracks
        ystr = "Number of Path Tracks"
        k = 6000
    }
    else {
        x = df$tottime
        ystr = "Simulated Runtime (seconds)"
        k = 10000
    }
    perc = length(which(df$ntracks<=k & df$suc =="2"))
    plot(df$alpha,x,col=colors,xlab=TeX('$\\alpha$'),ylab=TeX(ystr),main=TeX(paste(e, ' edges (', paste(perc," succesful runs w/ \u2264 6000 tracks)"), sep='')),ylim=c(0,6*1000*e),pch=16)
    if (legend==TRUE) {
        legend("top", legend=c("Success", "Failure"),col=c("red", "blue"), pch=c(16,16), cex=legsize,bty="n",y.intersp=2)
    }
    abline(v=1/(nodeCount*e),col="red") # lower bound
    abline(v=log10(rootCount)/e,col="blue") # upper
    abline(h=k,col="purple")
    if (predict == TRUE) {
        mod<-lm(ntracks~poly(alpha,2,raw=TRUE),df[df$suc=="100",])
        lines(df[df$suc=="100",]$alpha, predict(mod))
        }
    }

par(mfrow=c(3,2),oma=c(0,0,2,0))
createPlot(2,1)
createPlot(3,1)
createPlot(5,1)
createPlot(10,0.8)
createPlot(25,1)
createPlot(50,1.2,legend=TRUE)

title("Simulated number of tracks vs task success rate (1000 solutions, 3 nodes, 1 thread)",outer=TRUE)

# save plot

dev.copy(png, "../../../paper/PIX/trial_plot.png")
dev.off()
