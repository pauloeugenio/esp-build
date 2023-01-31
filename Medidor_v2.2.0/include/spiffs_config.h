/**
 * @file spiffs_config.h
 * @brief Arquivo cabeçalho para gerenciamento do SPIFFS
 *
 * @author --
 * @date 03 de Maio de 2022
 */

#ifndef _SPIFFS_CONFIG_
    #define _SPIFFS_CONFIG_ 

    #include <FS.h>
    #include "SPIFFS.h"

    /**
     * @brief Listar arquivos armazenados no filesystem [list_files()]
     * 
     * @param list_content Flag para permitir listar o conteúdo dos arquivos enquanto são listados
     */
    void list_files(bool list_content);

    /**
     * @brief Listar conteúdo do arquivo [list_file_content()]
     * 
     * @param file Arquivo para ter seu conteúdo listado
     */
    void list_file_content(File file);
    
    /**
     * @brief Abrir arquivo em append mode [open_file()]
     * 
     * @param filename Nome do arquivo a ser aberto
     * 
     * @return
     *     - Manipulador para o arquivo aberto
     */
    File open_file(String filename);
    
    /**
     * @brief Escrever linha no arquivo [write_line()]
     * 
     * @param file Manipulador do arquivo para ser escrito
     * @param line Linha a ser escrita no arquivo destino
     * 
     * @return
     *     - Manipulador para o arquivo aberto
     */
    File write_line(File &file, String line);

    /**
     * @brief Formatar SPIFFS [format_spiffs()]
     */
    void format_spiffs();



#endif // _SPIFFS_CONFIG_ 