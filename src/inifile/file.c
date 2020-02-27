/*
 *
 * handle inifile 
 *
 */

#include <stdlib.h>		/* for calloc free */
#include <string.h>		/* for strdup */
#include <errno.h> 		/* for errno */
#include <unistd.h>  		/* for usleep */
//#include "inifile.h"
#include "file.h"


int _handle_file_error(file_t *file, fpos_t *position)
{
	if (file && position) {
		if (feof(file->fp)) {
			file->eof = 1; /* true */
			clearerr(file->fp);
			usleep(1000);  /* why give it a breather... */
		} else if (ferror(file->fp)) {
			switch (errno) {
			case EAGAIN:
				usleep(1000);
				return 1;
			default:
				fprintf(stderr, "%s: %s (%d)", 
					__func__,
					strerror(errno), 
					errno);
			}
		} else {
			fprintf(stderr, "%s: unknow error %s (%d)",
				__func__,
				strerror(errno), 
				errno);
		}
		
		fsetpos(file->fp, position);
		return 0;                         /* false */
	} else {
		fprintf(stderr, "wront arguments\n");
		return 0;
	}
}

/* -----------------------------------------------
 * interface  
 * -----------------------------------------------
 */

file_t *_new_file(const char *name, const char *mode)
{
	file_t *file = (file_t *)calloc(1, sizeof(file_t));
	if (file) {
		if (name) {
			file->fp = fopen(name, mode);
			if (file->fp) {
				struct stat st;
				file->name = strdup(name); /* should we need to think about fail strdup or stat? */
				stat(file->name, &st);
				file->size = st.st_size;
			} else {
				fprintf(stderr, "%s: cannot open file %s\n",
					__func__, name);
				_destroy_file(&file);
			}
		} else {
			fprintf(stderr, "%s: cannot open file \
				without passing a name\n", __func__);
			_destroy_file(&file);
		}
	}
	return file;
}

void _destroy_file(file_t **file)
{
	if (file && *file) {
		if ((*file)->fp)
			fclose((*file)->fp);
		if ((*file)->name)
			free((*file)->name);
		if ((*file)->line)
			free((*file)->line);
		free(*file);
		*file = NULL;
	}
}

int _read_file(file_t *file, void **buf, uint32_t size, uint16_t count)
{
	if (file && file->fp && buf) {
		fpos_t position;
		/* should the caller free the memory? */
		if (!*buf && ((*buf = calloc(count, size)) == NULL)) return 0;

		fgetpos(file->fp, &position);
	try_again:
		if ((file->nread = fread(*buf, size, count, file->fp)) ==count) {
			file->read_bytes += (count * size);
			file->eof	  = 0;
			return 1;		/* true */
		}
		if (_handle_file_error(file, &position)) goto try_again;
	}
	return 0; 		/* false */
}

int _write_file(file_t *file, void *buf, uint32_t size, uint16_t count)
{
	if (file && file->fp && buf) {
		fpos_t position;
		fgetpos(file->fp, &position);
	try_again:
		if (fwrite(buf, size, count, file->fp) == count) {
			fflush(file->fp);
			file->write_bytes += (count * size);
			return 1;	/* true */
		}
		if (_handle_file_error(file, &position)) goto try_again;
	}
	return 0; 		/* false */
}

int _getpos_file(file_t *file, fpos_t *pos)
{
	if (file && file->fp && pos && 
		!fgetpos(file->fp, pos)) {
		return 1; /* true */
	}
	return 0; /* false */
}

int _setpos_file(file_t *file, fpos_t *pos)
{
	if (file && file->fp && pos &&
		!fsetpos(file->fp, pos))
		return 1; /* true */
	return 0; /* false */
}

int _stat_file(file_t *file, struct stat *st)
{
	if (file && file->fp && st &&
		!fstat(fileno(file->fp), st))
		return 1;
	return 0;
}

void _rewind_file(file_t *file)
{
	if (file && file->fp) 
		rewind(file->fp);
}

void _truncate_file(file_t *file)
{
	if (file && file->fp) {
		if (ftruncate(fileno(file->fp), 0) == -1)
			_rewind_file(file);
	}
}

int _isEOF_file(file_t *file)
{
 	return file ? file->eof : 0;
}


char *_getline_file(file_t *file)
{
	if (!file || !file->fp) {
		return NULL;
	} else {
		char *line = NULL;
		fpos_t position;
		size_t len = 0;

		fgetpos(file->fp, &position);
	try_again: /* getline will alloc memory and store the pointer in @file->line*/
		if (getline(&file->line, &file->line_len, file->fp) != -1) 
			return file->line;
		
		if (_handle_file_error(file, &position)) goto try_again;
			return NULL;
	}
}


