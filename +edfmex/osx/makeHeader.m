function makeHeader

%Generates the header files edf2mex.h which contains necessary mex structures and functions
%to read data from C structures defined in edf_data.h and load them into similarly organized
% mxArray structures.
%
% To use this, make sure that the edf header files edf.h, edftypes.h, edf_data.h are in the same
% directory, then type 'makeHeader' at the matlab command line. Make sure these are for the same
% version of edfapi.dll you intend to use. The output is a C header file, edf2mex.h.

%Kovach 2009

 

%RecTypes contains information about which structure type definitions to
%search for in the edf_data.h header. 


edfapi_version = '3.1 MacOS X Jul 13 2010';
%FSAMPLE type
RecTypes.FSAMPLE.codes = {'SAMPLE_TYPE'} ;                  %Constants which code for an event type that uses this structure
RecTypes.FSAMPLE.allf_code= 'fs';                       %field of ALLF_DATA union corresponding to this structure


%FEVENT type
RecTypes.FEVENT.codes = {'STARTPARSE','ENDPARSE','BREAKPARSE','STARTBLINK ','ENDBLINK','STARTSACC',...
    'ENDSACC','STARTFIX','ENDFIX','FIXUPDATE','MESSAGEEVENT','STARTSAMPLES','ENDSAMPLES','STARTEVENTS','ENDEVENTS'};
RecTypes.FEVENT.allf_code= 'fe';

% RecTypes.CONTROL.codes = {'STARTSAMPLES','ENDSAMPLES','STARTEVENTS','ENDEVENTS',};


%IOEVENT type
RecTypes.IOEVENT.codes= {'BUTTONEVENT','INPUTEVENT','LOST_DATA_EVENT'}; 
RecTypes.IOEVENT.allf_code= 'io'; 

% RecTypes.LOST_DATA.codes = {'LOST_DATA_EVENT'};

%RECORDINGS type
RecTypes.RECORDINGS.codes = {'RECORDING_INFO'};
RecTypes.RECORDINGS.allf_code = 'rec';


%%%%%%%%%%%%%%%%%%%%%%%
%
% Parse edf_data.h to find all data structures described in RecTypes
%

fid = fopen('edf_data.h');
header1 = char(fread(fid,'*uchar')');
fclose(fid);

rectype = fieldnames(RecTypes);

for i = 1:length(rectype)   
    RecTypes.(rectype{i}).vars = getStructTypes(header1,rectype{i}); %C structure defined in edf_data.h
end


%%%%%%
%
% Start making the header file, edf2mex.h
%

fid = fopen('edf2mex.h','w');

fprintf(fid,'\n\nchar *  build_version = "%s";\n\n',edfapi_version);

fprintf(fid,'\nclass BuildMexArrays : public BuildMexArraysBaseClass\n{\n\n');



fprintf(fid,'\npublic:\n\nint PopulateArrays( int Offset = 0, int MaxRec = 0 );\n');
fprintf(fid,'\n~BuildMexArrays();\n');


for i = 1:length(rectype)
    fprintf(fid,'\nint Nof%s;',rectype{i});
    fprintf(fid,'\nmxArray * mx%s;',rectype{i});
    
end

% % fprintf(fid,'\n\nstd::string build_version ("%s");',edfapi_version);
% fprintf(fid,'\n\nstd::string build_version;');
% fprintf(fid,'\nbuild_version.assign("%s");',edfapi_version);
% fprintf(fid,'\n\string curr_version (%i,"0");',length(edfapi_version));
% fprintf(fid,'\n\nchar * curr_version = edf_get_version();');
% 
% fprintf(fid,'\n\nif memcmp(build_version,curr_version,%i) mexErrMsgTxt("EDFMEX is compiled for version " + curr_version + " of EDFAPI.\\n You must use run makeHeader.m and recompile EDFMEX.");\n',length(edfapi_version));

for i = 1:length(rectype)

    

    if ~isfield(RecTypes.(rectype{i}),'vars'), continue, end;
    fprintf(fid,'\n\n\n/*************************************\n*\tFunctions and objects to handle events of type %s\n*************************************/',rectype{i});

      recfields = {RecTypes.(rectype{i}).vars.var};
      recfmt = {RecTypes.(rectype{i}).vars.type};
      recfmt = regexprep(recfmt,'byte','uint8');
      recfmt = regexprep(recfmt,'float','single');
      
      
    allf_code = RecTypes.(rectype{i}).allf_code;

    %Create a structure that contains data type arrays (which can be passed
    %to mex)

        fprintf(fid,'\n\n struct \t%stype\n{\n\tconst char * fieldnames[%i];',rectype{i}, length(recfields));
        if ~isempty(strfind(lower(rectype{i}),'sample'))

            for rf = 1:length(recfields)    
                    fprintf(fid,'\n\t%s* %s;',recfmt{rf},recfields{rf});
            end
        end
        fprintf(fid,'\n\n\t%stype(){\n',rectype{i});
        for j = 1:length(recfields)
            fprintf(fid,'\n\t\t\tfieldnames[%i] = "%s";',j-1,recfields{j});
        end
        
        fprintf(fid,'\n\t};\n\n} str%s;',rectype{i});
    
     
     %%%%%%%%%%%%%%%%%%%%%%%
     %
     % Functions to append data 
     %
    fprintf(fid,'\n\nint mx%sappend()\n{',rectype{i});
     
    if ~isempty(strfind(lower(rectype{i}),'sample'))


        for j = 1:length(recfields)
            dim = RecTypes.(rectype{i}).vars(j).dim ; 
            if dim(1) > 1
                 fprintf(fid,'\n\tmemcpy( &(str%s.%s[%i*Nof%s]) ,  &(CurrRec->%s.%s[0]), %i*sizeof(%s));',...
                    rectype{i},recfields{j},dim(1),rectype{i},allf_code ,recfields{j},dim(1),recfmt{j});             
            else
%                     fprintf(fid,'\n\t str%s.%s[Nof%s] = CurrRec->%s.%s;',...
                    fprintf(fid,'\n\tmemcpy( &(str%s.%s[Nof%s]) , &(CurrRec->%s.%s), %i*sizeof(%s));',...
                        rectype{i},recfields{j},rectype{i},allf_code ,recfields{j},dim(1),recfmt{j});             
            end


        end
    else
        
        for j = 1:length(recfields)
            dim = RecTypes.(rectype{i}).vars(j).dim ; 

            if strcmp(recfmt{j} , 'LSTRING')  
                   fprintf(fid,'\n\n\tif (CurrRec->%s.%s != NULL) mxSetField(mx%s, Nof%s, "%s",  mxCreateString(  &(CurrRec->%s.%s->c) ) );',...
                       allf_code ,recfields{j},rectype{[i i]}, recfields{j},allf_code ,recfields{j});

            else

                fprintf(fid,'\n\n\tmxSetFieldByNumber(mx%s, Nof%s, %i,  mxCreateNumericMatrix(%i,1, mx%s_CLASS,mxREAL));',...
                rectype{[i i]},j-1,dim(1),upper(recfmt{j}));
                fprintf(fid,'\n\tmemcpy(mxGetData(mxGetFieldByNumber(mx%s, Nof%s, %i)) ,&(CurrRec->%s.%s),%i*sizeof(mx%s_CLASS) );',...
                rectype{[i i]},j-1,allf_code,recfields{j},dim,upper(recfmt{j}));
            end
          

        end
        fprintf(fid,'\n\tmxSetField(mx%s, Nof%s, "codestring",  mxCreateString(GetRecordCodeStr(GetDataCode())));',rectype{[i i]});
   
    end
                
        fprintf(fid,'\n\n\tNof%s++;',rectype{i});

        fprintf(fid,'\n\n\treturn 0;\n\n};');
        
    
end    


fprintf(fid,'\n\nchar * GetRecordTypeStr(int CODE)\n{');
fprintf(fid,'\n\n\tswitch(CODE) {');

for i = 1:length(rectype)
    reccodes = RecTypes.(rectype{i}).codes;
    
    for j = 1:length(reccodes)
        fprintf(fid,'\n\t\tcase %s:',reccodes{j});
    end
    fprintf(fid,'\n\t\t\treturn "%s";',rectype{i});
end
    
fprintf(fid,'\n\t}\n\n\treturn NULL;');
    
fprintf(fid,'\n\n};\n\n');
        
%%%%%%%%%%%%
%
%   Switch that decides which append function to call based on code

fprintf(fid,'\n\nint AppendRecord()\n{');
fprintf(fid,'\n\n\tCurrRec = edf_get_float_data( edfptr);'); 

fprintf(fid,'\n\n\tswitch(DataCode) {');

for i = 1:length(rectype)
    if ~isfield(RecTypes.(rectype{i}),'vars'), continue, end;
    reccodes = RecTypes.(rectype{i}).codes;
    
    for j = 1:length(reccodes)
        fprintf(fid,'\n\t\tcase %s:',reccodes{j});
    end
    fprintf(fid,'\n\t\t\treturn mx%sappend();',rectype{i});
end
    
fprintf(fid,'\n\t}\n\n\treturn 0;');
    
fprintf(fid,'\n\n};\n\n');

%%%%%%%%%%
%
% Function that returns a string descriptor for each code

fprintf(fid,'\n\nchar * GetRecordCodeStr(int CODE)\n{');
fprintf(fid,'\n\n\tswitch(CODE){');

for i = 1:length(rectype)
    reccodes = RecTypes.(rectype{i}).codes;
    
    for j = 1:length(reccodes)
        fprintf(fid,'\n\t\tcase %s:',reccodes{j});
        fprintf(fid,'\n\t\t\treturn "%s";',reccodes{j});
    end
end
    
fprintf(fid,'\n\t}\n\n\treturn NULL;');
    
fprintf(fid,'\n\n};\n\n');



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CONSTRUCTOR

fprintf(fid,'\n\nBuildMexArrays( char * filenamein , int consistency_check , int load_events  , int load_samples)\n{');

for j = 1:length(rectype)
    
    fprintf(fid,'\n\t RecordTypes[%i] = "%s";',j-1,rectype{j});
end


fprintf(fid,'\n\n\tInitialize( filenamein , consistency_check , load_events  , load_samples);\n');

fprintf(fid,'\n\n\tOutputMexObject = mxCreateStructMatrix(1,1,%i,RecordTypes);\n',length(rectype));

for i = 1:length(rectype)
    
    fprintf(fid, '\n\n\n\tNof%s = 0;', rectype{i});

    if ~isfield(RecTypes.(rectype{i}),'vars'), continue, end;
    
      recfields = {RecTypes.(rectype{i}).vars.var};
      recfmt = {RecTypes.(rectype{i}).vars.type};
      recfmt = regexprep(recfmt,'byte','uint8');
      recfmt = regexprep(recfmt,'float','single');
      
     
    if ~isempty(strfind(lower(rectype{i}),'sample'))

        fprintf(fid,'\n\n\t mx%s = mxCreateStructMatrix(1,1,%i,str%s.fieldnames);',rectype{i},length(recfields),rectype{i});

        for j = 1:length(recfields)

            dim = RecTypes.(rectype{i}).vars(j).dim ; 
                    fprintf(fid,'\n\n\tmxSetFieldByNumber( mx%s,0,%i,  mxCreateNumericMatrix(%i,Nrec, mx%s_CLASS,mxREAL) );',...
                      rectype{i},j-1,dim(1), upper(recfmt{j}));
                    fprintf(fid,'\n\tstr%s.%s = (%s *) mxGetData( mxGetFieldByNumber(mx%s,0,%i) );',...
                    rectype{i},recfields{j},recfmt{j},rectype{i},j-1);


        end


        
    else
        fprintf(fid,'\n\n\t mx%s = mxCreateStructMatrix(1,Nrec,%i,str%s.fieldnames);',rectype{i},length(recfields),rectype{i});
        fprintf(fid,'\n\n\t mxAddField(mx%s,"codestring");',rectype{i});

        
    end
    
    

end

fprintf(fid,'\n\n};');

%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Function that sets the output structure.

fprintf(fid,'\n\nint CreateMexStruc()\n{');


for i = 1:length(rectype)
    if ~isfield(RecTypes.(rectype{i}),'vars'), continue, end;

    if ~isempty(strfind(lower(rectype{i}),'sample'))
        recfields = {RecTypes.(rectype{i}).vars.var};
%         recfmt = {RecTypes.(rectype{i}).vars.type};
%         recfmt = regexprep(recfmt,'byte','uint8');
%         recfmt = regexprep(recfmt,'float','single');

        for j = 1:length(recfields) 
%             dim = RecTypes.(rectype{i}).vars(j).dim ; 
            fprintf(fid,'\n\n\tmxSetData( mxGetFieldByNumber( mx%s,0,%i) ,str%s.%s );',rectype{i},j-1,rectype{i},recfields{j});
            fprintf(fid,'\n\tmxSetN(mxGetFieldByNumber(mx%s,0,%i),Nof%s);',rectype{i},j-1,rectype{i});
        end
        fprintf(fid,'\n');
    else

     fprintf(fid,'\n\n\tmxSetN(mx%s,Nof%s);',rectype{[i i]});
    end
     fprintf(fid,'\n\tmxSetField(OutputMexObject,0,"%s",mx%s);',rectype{[i i]});

end
fprintf(fid,'\n\n\treturn 0;\n};');
fprintf(fid,'\n\n};//end of class def\n\n');

fclose(fid);

%%%%%%%%%%%%%%%%%

function D = getStructTypes(str,strDef)

% This function parses the variables inside the object identified by
% strDef.

a = regexp(str,sprintf('{([^}]*)}\\s*%s',strDef),'tokens');
b = regexp(a{1}{1},'\n[^\w/]*(\w+)\s*([^;]*)','tokens');
bs = cat(1,b{:});
types = bs(:,1);
vars = bs(:,2);

c = regexp(vars,'(\w+)\[?(\d*\s*\d*)\]?','tokens');


D = struct([]);
for i = 1:length(c)
    for j = 1:length(c{i})
        D(end+1).var = c{i}{j}{1};
        D(end).type = types{i};
        if isempty( c{i}{j}{2})
            D(end).dim = 1;
        else
            D(end).dim = str2double(c{i}{j}{2});
        end
    end
end
