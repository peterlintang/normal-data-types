function varargout = s2dplot(varargin)
% S2DPLOT MATLAB code for s2dplot.fig
%      S2DPLOT, by itself, creates a new S2DPLOT or raises the existing
%      singleton*.
%
%      H = S2DPLOT returns the handle to a new S2DPLOT or the handle to
%      the existing singleton*.
%
%      S2DPLOT('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in S2DPLOT.M with the given input arguments.
%
%      S2DPLOT('Property','Value',...) creates a new S2DPLOT or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before s2dplot_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to s2dplot_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help s2dplot

% Last Modified by GUIDE v2.5 16-Dec-2024 15:46:07

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @s2dplot_OpeningFcn, ...
                   'gui_OutputFcn',  @s2dplot_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before s2dplot is made visible.
function s2dplot_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to s2dplot (see VARARGIN)

% Choose default command line output for s2dplot
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes s2dplot wait for user response (see UIRESUME)
% uiwait(handles.GUI_window);


% --- Outputs from this function are returned to the command line.
function varargout = s2dplot_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
Xmin = str2double(get(handles.Xmin, 'string'));
Xmax = str2double(get(handles.Xmax, 'string'));

x = linspace(Xmin, Xmax, 200);
f = sinc(x*pi);
plot(x, f, 'b');
grid on;
title('sinc(\pi*x) = sin(\pi*x)/(\pi*x)');
xlabel('x');
ylabel('f(x)=sinc(\pi*x)');
axis([Xmin, Xmax, -.25, 1.1]);


function Xmin_Callback(hObject, eventdata, handles)
% hObject    handle to Xmin (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Xmin as text
%        str2double(get(hObject,'String')) returns contents of Xmin as a double


% --- Executes during object creation, after setting all properties.
function Xmin_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Xmin (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function Xmax_Callback(hObject, eventdata, handles)
% hObject    handle to Xmax (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Xmax as text
%        str2double(get(hObject,'String')) returns contents of Xmax as a double


% --- Executes during object creation, after setting all properties.
function Xmax_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Xmax (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton2.
function pushbutton2_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
delete(handles.GUI_window);
run('SOUND_hear.m');
A=imread('GC2011.jpg');
image(A);
msgbox('All done Well Done!');

