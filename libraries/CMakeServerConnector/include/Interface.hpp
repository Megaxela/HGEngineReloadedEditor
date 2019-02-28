#pragma once

#include <string>

/**
 * @brief Class, that describes interface to
 * cmake pipe.
 */
class Interface
{
public:

    /**
     * @brief Constructor.
     */
    Interface();

    /**
     * @brief Destructor.
     */
    ~Interface();

    /**
     * @brief Is connection opened.
     */
    bool isConnected() const;

    /**
     * @brief Perform connection to cmake pipe.
     * If there is any initialization error - std::runtime_error
     * exception will be throw.
     * @param path Path to cmake pipe.
     * @return Status.
     */
    bool connect(const std::string& path);

    /**
     * @brief Method for perform disconnection.
     */
    void disconnect();

    /**
     * @brief Method for writing data to cmake pipe.
     * @param s Data.
     */
    void write(const std::string& s);

    /**
     * @brief Method for reading data from cmake pipe.
     * @return Data.
     * @return Read success. Somethong wrong if false.
     */
    bool read(std::string& out);

private:

    int m_handle;
};

