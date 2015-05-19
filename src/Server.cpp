#include "Server.h"
#include <sys/queue.h>
#include <evhttp.h>

#define PARSE_URI(request, params) { \
    const char *query_part = evhttp_uri_get_query(request->uri_elems); \
    evhttp_parse_query_str(query_part, &params); \
}


#define PARAM_GET_STR(var, params, name, mendatory) { \
    var = evhttp_find_header(&params_get, name); \
    if (!var && mendatory) { \
        http_send(request, "<err message=\"field '" name "' is mendatory\"/>\n"); \
        return; \
    } \
}

inline static void output_xml_header(struct evbuffer *buffer) {
    evbuffer_add_printf(buffer, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    evbuffer_add_printf(buffer, "<root>\n");
}

inline static void output_xml_footer(struct evbuffer *buffer) {
    evbuffer_add_printf(buffer, "</root>\n");
}


inline static void trad_output_xml(std::string trad, struct evbuffer *buffer) {
    evbuffer_add_printf(buffer, "<trad><![CDATA[");
    evbuffer_add_printf(buffer, "%s", trad.c_str());
    evbuffer_add_printf(buffer, "]]></trad>\n");
}

inline static void guess_script_output_xml(std::string script, struct evbuffer *buffer) {
    evbuffer_add_printf(buffer, "<script><![CDATA[");
    evbuffer_add_printf(buffer, "%s", script.c_str());
    evbuffer_add_printf(buffer, "]]></script>\n");
} 


inline static void change_script_output_xml(std::string otherscript, struct evbuffer *buffer) {
    evbuffer_add_printf(buffer, "<alternateScript><![CDATA[");
    evbuffer_add_printf(buffer, "%s", otherscript.c_str());
    evbuffer_add_printf(buffer, "]]></alternateScript>\n");
} 



inline static void simp_output_xml(std::string simp, struct evbuffer *buffer) {
    evbuffer_add_printf(buffer, "<simp><![CDATA[");
    evbuffer_add_printf(buffer, "%s", simp.c_str());
    evbuffer_add_printf(buffer, "]]></simp>\n");
} 

inline static void romanization_output_xml(std::string jyutping, struct evbuffer *buffer) {
    evbuffer_add_printf(buffer, "<romanization><![CDATA[");
    evbuffer_add_printf(buffer, "%s", jyutping.c_str());
    evbuffer_add_printf(buffer, "]]></romanization>\n");
} 





/**
 *
 */
static void http_send(struct evhttp_request *request, const char *fmt, ...) {
    struct evbuffer *buffer = evbuffer_new();
    evhttp_add_header(request->output_headers, "Content-Type", "TEXT/XML; charset=UTF8");
    va_list ap;
    va_start(ap, fmt);
    evbuffer_add_vprintf(buffer, fmt, ap);
    va_end(ap);
    evhttp_send_reply(request, HTTP_OK, "", buffer);
    evbuffer_free(buffer);
}



/**** uri: *
 *
 */
static void http_callback_default(struct evhttp_request *request, void *data) {
    evhttp_send_error(request, HTTP_NOTFOUND, "Service not found");
}

/**** uri: /pinyin?str=*
 *
 */
static void http_pinyin_callback(struct evhttp_request *request, void *data) {

    //parse uri
    struct evkeyvalq params_get;
    PARSE_URI(request, params_get);
    SinoparserServer* serv = (SinoparserServer*) data;


    //get "str"
    char const *str;
    PARAM_GET_STR(str, &params_get, "str", true);
    serv->parser.change_text((char*)str);
    serv->parser.parse_against_index(serv->db.mandarin_words);
    std::string pinyin = serv->parser.romanize();

    //prepare output
    struct evbuffer *buffer = evbuffer_new();

    output_xml_header(buffer);
    romanization_output_xml(pinyin, buffer);
    output_xml_footer(buffer);

    //send
    evhttp_add_header(request->output_headers, "Content-Type", "TEXT/XML; charset=UTF8");

    evhttp_send_reply(request, HTTP_OK, "", buffer);

    evhttp_clear_headers(&params_get);
    evbuffer_free(buffer);
}

/**** uri: /jyutping?str=*
 *
 */
static void http_jyutping_callback(struct evhttp_request *request, void *data) {

    //parse uri
    struct evkeyvalq params_get;
    PARSE_URI(request, params_get);
    SinoparserServer* serv = (SinoparserServer*) data;


    //get "str"
    char const *str;
    PARAM_GET_STR(str, &params_get, "str", true);
    serv->parser_cantonese.change_text((char*)str);
    serv->parser_cantonese.parse_against_index(serv->db.cantonese_words);
    std::string jyutping = serv->parser_cantonese.romanize();

    //prepare output
    struct evbuffer *buffer = evbuffer_new();

    output_xml_header(buffer);
    romanization_output_xml(jyutping, buffer);
    output_xml_footer(buffer);

    //send
    evhttp_add_header(request->output_headers, "Content-Type", "TEXT/XML; charset=UTF8");

    evhttp_send_reply(request, HTTP_OK, "", buffer);

    evhttp_clear_headers(&params_get);
    evbuffer_free(buffer);
}


/**** uri: /change_script?str=*
 *
 */
static void http_change_script_callback(struct evhttp_request *request, void *data) {

    //parse uri
    struct evkeyvalq params_get;
    PARSE_URI(request, params_get);
    SinoparserServer* serv = (SinoparserServer*) data;


    //get "str"
    char const *str;
    PARAM_GET_STR(str, &params_get, "str", true);
    serv->parser.change_text((char*)str);
    serv->parser.parse_against_index(serv->db.mandarin_words);
    std::string change_script = serv->parser.change_script();

    //prepare output
    struct evbuffer *buffer = evbuffer_new();
 
    output_xml_header(buffer);
    change_script_output_xml(change_script, buffer);
    output_xml_footer(buffer);


    //send
    evhttp_add_header(request->output_headers, "Content-Type", "TEXT/XML; charset=UTF8");

    evhttp_send_reply(request, HTTP_OK, "", buffer);

    evhttp_clear_headers(&params_get);
    evbuffer_free(buffer);
}



/**** uri: /trad?str=*
 *
 */
static void http_trad_callback(struct evhttp_request *request, void *data) {

    //parse uri
    struct evkeyvalq params_get;
    PARSE_URI(request, params_get);
    SinoparserServer* serv = (SinoparserServer*) data;


    //get "str"
    char const *str;
    PARAM_GET_STR(str, &params_get, "str", true);

    serv->parser.change_text((char*)str);
    serv->parser.parse_against_index(serv->db.mandarin_words);
    std::string trad = serv->parser.trad();

    //prepare output
    struct evbuffer *buffer = evbuffer_new();
 
    output_xml_header(buffer);
    trad_output_xml(trad, buffer);
    output_xml_footer(buffer);


    //send
    evhttp_add_header(request->output_headers, "Content-Type", "TEXT/XML; charset=UTF8");

    evhttp_send_reply(request, HTTP_OK, "", buffer);

    evhttp_clear_headers(&params_get);
    evbuffer_free(buffer);
}



/**** uri: /simp?str=*
 *
 */
static void http_simp_callback(struct evhttp_request *request, void *data) {

    //parse uri
    struct evkeyvalq params_get;
    PARSE_URI(request, params_get);
    SinoparserServer* serv = (SinoparserServer*) data;


    //get "str"
    char const *str;
    PARAM_GET_STR(str, &params_get, "str", true);
    serv->parser.change_text((char*)str);
    serv->parser.parse_against_index(serv->db.mandarin_words);
    std::string simp = serv->parser.simp();

    //prepare output
    struct evbuffer *buffer = evbuffer_new();
 
    output_xml_header(buffer);
    simp_output_xml(simp, buffer);
    output_xml_footer(buffer);


    //send
    evhttp_add_header(request->output_headers, "Content-Type", "TEXT/XML; charset=UTF8");

    evhttp_send_reply(request, HTTP_OK, "", buffer);

    evhttp_clear_headers(&params_get);
    evbuffer_free(buffer);
}


/**** uri: /guess_script?str=*
 *
 */
static void http_guess_script_callback(struct evhttp_request *request, void *data) {

    //parse uri
    struct evkeyvalq params_get;
    PARSE_URI(request, params_get);
    SinoparserServer* serv = (SinoparserServer*) data;


    //get "str"
    char const *str;
    PARAM_GET_STR(str, &params_get, "str", true);
    serv->parser.change_text((char*)str);
    serv->parser.parse_against_index(serv->db.mandarin_words);

    std::string guess_script = ""; 
    bool script = serv->parser.guess_script();
    if (script == TRADITIONAL_SCRIPT) {
        guess_script = "traditional";
    }  else if (script == SIMPLIFIED_SCRIPT) {
        guess_script = "simplified";
    }

    //prepare output
    struct evbuffer *buffer = evbuffer_new();
 
    output_xml_header(buffer);
    guess_script_output_xml(guess_script, buffer);
    output_xml_footer(buffer);


    //send
    evhttp_add_header(request->output_headers, "Content-Type", "TEXT/XML; charset=UTF8");

    evhttp_send_reply(request, HTTP_OK, "", buffer);

    evhttp_clear_headers(&params_get);
    evbuffer_free(buffer);
}


/**** uri: /all?str=*
 *
 */
static void http_all_callback(struct evhttp_request *request, void *data) {

    //parse uri
    struct evkeyvalq params_get;
    PARSE_URI(request, params_get);
    SinoparserServer* serv = (SinoparserServer*) data;


    //get "str"
    char const *str;
    PARAM_GET_STR(str, &params_get, "str", true);
    serv->parser.change_text((char*)str);
    serv->parser.parse_against_index(serv->db.mandarin_words);

    std::string guess_script = ""; 
    bool script = serv->parser.guess_script();
    if (script == TRADITIONAL_SCRIPT) {
        guess_script = "traditional_script";
    }  else if (script == SIMPLIFIED_SCRIPT) {
        guess_script = "simplified_script";
    }

    //prepare output
    struct evbuffer *buffer = evbuffer_new();
 
    output_xml_header(buffer);
        guess_script_output_xml(guess_script, buffer);
        romanization_output_xml(serv->parser.romanize(), buffer);
        change_script_output_xml(serv->parser.change_script(), buffer);
    output_xml_footer(buffer);


    //send
    evhttp_add_header(request->output_headers, "Content-Type", "TEXT/XML; charset=UTF8");

    evhttp_send_reply(request, HTTP_OK, "", buffer);

    evhttp_clear_headers(&params_get);
    evbuffer_free(buffer);
}




/**
 *
 */

SinoparserServer::SinoparserServer(std::string address, int port, Database& ndb) {

    db = ndb;

    struct event_base *base = event_init();
    struct evhttp *server = evhttp_new(base);
    int res = evhttp_bind_socket(server, address.c_str(), port);

    if (res != 0) {
        std::cout <<  "[ERROR] Could not start http server!" << std::endl;
        return;
    }
    db.debug();

    evhttp_set_gencb(server, http_callback_default, this);
    evhttp_set_cb(server, "/pinyin", http_pinyin_callback, this);
    evhttp_set_cb(server, "/jyutping", http_jyutping_callback, this);
    evhttp_set_cb(server, "/trad", http_trad_callback, this);
    evhttp_set_cb(server, "/simp", http_simp_callback, this);
    evhttp_set_cb(server, "/guess_script", http_guess_script_callback, this);
    evhttp_set_cb(server, "/change_script", http_change_script_callback, this);
    evhttp_set_cb(server, "/all", http_all_callback, this);

    event_base_dispatch(base);
}

/**
 *
 *
 */
SinoparserServer::~SinoparserServer() {

}
