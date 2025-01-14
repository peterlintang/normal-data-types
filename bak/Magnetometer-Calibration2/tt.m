D=importdata('sensor_data.txt');
[A,b,expmfs] = magcal(D);
C=(D-b)*A;
figure(1)
plot3(D(:,1),D(:,2),D(:,3),'LineStyle','none','Marker','X','MarkerSize',8)
hold on
grid(gca,'on')
plot3(C(:,1),C(:,2),C(:,3),'LineStyle','none','Marker', ...
            'o','MarkerSize',8,'MarkerFaceColor','r')
axis equal
xlabel('x')
ylabel('y')
zlabel('z')
legend('Uncalibrated Samples', 'Calibrated Samples','Location', 'southoutside')
title("Uncalibrated vs Calibrated" + newline + "Magnetometer Measurements")
hold off
