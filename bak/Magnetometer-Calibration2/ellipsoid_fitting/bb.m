% 创建一个简单的Simulink模型
mdl = Simulink.Model('MyEmbeddedSystemModel');
 
% 添加一个简单的数据处理块
dataBlock = Simulink.Block('Data Generator');
set_param(dataBlock, 'NumDataPorts', 1);
set_param(dataBlock, 'OutputData', [1:10]);
 
% 添加一个简单的状态机
stateflowBlock = Simulink.Block('Stateflow');
set_param(stateflowBlock, 'StateflowChart', 'path_to_stateflow_chart.sfc');
 
% 将块添加到模型中
Simulink.Document.addBlock(mdl, dataBlock);
Simulink.Document.addBlock(mdl, stateflowBlock);
 
% 连接模型的块
Simulink.Document.connect(mdl, 'Data Generator|Data Out:1', 'Stateflow|In1');
 
% 编译模型
Simulink.Document.compile(mdl);
 
% 导出硬件描述文件
export_spec('MyExportSpecification');
set_param('MyExportSpecification', 'HardwareImplementation', 'Embedded C');
set_param('MyExportSpecification', 'FileCreationTime', 'Now');
export('MyEmbeddedSystemModel', 'MyExportSpecification');
 
% 现在可以将生成的代码集成到嵌入式系统开发环境中
