# MAKE DIGEST API WITH ARDUINO

<div id="top"></div>

<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]
[!["Buy Me A Coffee"](https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png)](https://www.buymeacoffee.com/gturturro)

<!-- PROJECT LOGO -->
<div align="center">
  <a href="https://github.com/zEhmsy/Arduino-Digest-Api/project_icon">
    <img src="project_icon.png" alt="Logo" width="80" height="80">
  </a>

<h3 align="center">Arduino IP Device API</h3>

  <p align="center">
    This sketch connects to an IP device with digest authentication using an Arduino WIZnet Ethernet shield, and sends API requests to it.
    <br />
    <a href="https://github.com/zEhmsy/Arduino-Digest-Api"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/zEhmsy/Arduino-Digest-Api">View Demo</a>
    ·
    <a href="https://github.com/zEhmsy/Arduino-Digest-Api/issues">Report Bug</a>
    ·
    <a href="https://github.com/zEhmsy/Arduino-Digest-Api/issues">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li>
        <a href="#detailed-function-explanation">Detailed Function Explanation</a>
        <ul>
        <li><a href="#gethttprequestethernetclient-client-const-string-uri">getHttpRequest</a></li>
        <li><a href="#posthttprequestethernetclient-client-const-string-uri-const-string-content">postHttpRequest</a></li>
        <li><a href="#getcnonceconst-int-len">getCNonce</a></li>
        <li><a href="#getdigestauthstring-responsestring-const-string-username-const-string-password-const-string-uri-unsigned-int-counter">getDigestAuth</a></li>
        <li><a href="#getcontentaftercontentlengthconst-string-httpresponse">getContentAfterContentLength</a></li>
      </ul>
    </li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#hardware-compatibility-and-future-versions">Hardware Compatibility and Future Versions</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

Digest authentication is a method utilized by web servers to authenticate users. It offers a more secure alternative to basic authentication by sending hashed passwords instead of plaintext ones over the network. Here's a brief overview of how digest authentication works:

1. **Client Request**: The client sends a request to access a protected resource on the server.

2. **Server Challenge**: The server responds with a 401 Unauthorized status code along with a challenge that includes a nonce (a unique value), realm (authentication domain), and other parameters.

3. **Client Authentication**: The client computes a hash of the username, password, and other parameters along with the server challenge. This hash is sent back to the server as part of the Authorization header in subsequent requests.

4. **Server Verification**: Upon receiving the hash from the client, the server verifies it by recomputing the hash using the same parameters and comparing it with the received hash. If they match, the server grants access to the requested resource.

This project implements digest authentication to establish a secure connection between the Arduino microcontroller and the IP device. It follows the RFC 2617 standard to generate and verify the necessary authentication credentials.


<!--[Product Name Screen Shot][product-screenshot]-->

<p align="right">(<a href="#top">back to top</a>)</p>



### Built With

* [SPI Library](https://www.arduino.cc/en/reference/SPI)
* [Ethernet Library](https://www.arduino.cc/en/reference/Ethernet)
* [MD5 Library](https://github.com/tzikis/ArduinoMD5)

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->
## Usage

This sketch is designed to connect to an IP device with digest authentication and send API requests to it. It can be used with any microcontroller equipped with an Ethernet shield.


To use this sketch with your specific setup, you'll need to modify certain parts of the code to adapt it to your situation. Specifically, you'll need to update the string fields found in the `config.h` file. Here's a brief explanation of each field:

1. `username`: Replace `<USERNAME>` with the username required for authentication with your IP device.

2. `password`: Replace `<PASSWORD>` with the password required for authentication with your IP device.

3. `server_pointer`: Replace `http://192.168.1.200` with the address of your IP device. Make sure to include the appropriate protocol (use `http://`, Unlike the `https://` because it has not been tested) and the IP address or domain name of your device.

4. `uri`: You have uncomment the `uri` field and replace `/cgi-bin/example` with the specific API endpoint of your IP device if needed.

Once you have updated these fields in the `config.h` file, upload the sketch to your Arduino board and open the serial monitor to observe the output.


<p align="right">(<a href="#top">back to top</a>)</p>

<!-- ROADMAP -->

## Roadmap

- Implementing error handling mechanisms
- Enhancing API request functionality
- Adding support for additional authentication methods

See the [open issues](https://github.com/zEhmsy/Arduino-Digest-Api/issues) for a full list of proposed features (and known issues).

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- GETTING STARTED -->

## Getting Started

### Prerequisites & Installation

1. Connect the Ethernet shield to the Arduino board.
2. Clone the repository and open the sketch in the Arduino IDE.
3. Set the appropriate IP address, MAC address, DNS, gateway, and subnet mask for your network.
4. Upload the sketch to your Arduino board.
5. Open the serial monitor to view the output.

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- DETAILS -->
## Detailed Function Explanation

In this section, we provide a detailed explanation of the various functions implemented in the code. While the code could be further optimized, it has been designed with educational purposes in mind.

### `getHttpRequest(EthernetClient& client, const String& uri)`

This function performs a GET request to the server specified by `uri` using the provided `EthernetClient`. It establishes a connection with the server, sends the request, and waits for the response. Once the response is received, it returns it as a string.

### `postHttpRequest(EthernetClient& client, const String& uri, const String& content)`

Similar to `getHttpRequest`, this function performs a POST request to the server specified by `uri` using the provided `EthernetClient`. It includes the `content` in the request body. It establishes a connection with the server, sends the request, and waits for the response. Once the response is received, it returns it as a string.

### `getCNonce(const int len)`

This function generates a random string of alphanumeric characters of length `len`. It is primarily used in generating the client nonce (cnonce) for digest authentication.

### `getDigestAuth(String& responseString, const String& username, const String& password, const String& uri, unsigned int counter)`

This function generates the digest authentication header required for authenticating with the server. It parses the server's challenge response (`responseString`) to extract the realm, nonce, and other parameters required for digest authentication. It then computes the MD5 hash of the username, password, and other parameters according to RFC 2617 specifications and constructs the authorization header.

### `getContentAfterContentLength(const String& httpResponse)`

This function extracts the content of the HTTP response after the `Content-Length` header. It is used to retrieve the actual API response from the server's HTTP response.

These functions together facilitate the communication between the Arduino microcontroller and the IP device, enabling secure authentication and data exchange over the network.


<!-- CONTRIBUTING -->

## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- FEEDBACK-->
## Hardware Compatibility and Future Versions

It's important to note that this code is designed to work specifically with Arduino boards equipped with an Ethernet connection. This includes boards like the Arduino Uno with an Ethernet shield or the Arduino Mega with built-in Ethernet support. The code utilizes the Ethernet library for network communication.

If you're using a different type of board, such as the ESP32 or ESP8266, which are commonly used for WiFi connectivity, this code won't be directly compatible. However, if this project receives positive feedback and demand for support on WiFi-enabled boards, a separate version of the code could be developed to accommodate these platforms.

In a potential future version for WiFi-enabled boards, the code would likely utilize libraries such as WiFiClient for communication over WiFi networks. This would enable compatibility with a broader range of microcontroller boards, expanding the reach of the project to a wider audience.

For now, this project serves as a functional demonstration of digest authentication implementation on Arduino boards with Ethernet connectivity, with potential future iterations to support additional hardware platforms. [If yu like it! Leave a star ⭐️](https://github.com/zEhmsy/Arduino-Digest-Api/stargazers)


<!-- LICENSE -->

## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- CONTACT -->

## Contact

Giuseppe Tururro - [WHOAMI?](https://zehmsy.github.io/digital-cv/) - [@turturrogiuseppe](https://www.instagram.com/turturrogiuseppe/) - info.g.turturro@gmail.com

Project Link: [https://github.com/zEhmsy/Arduino-Digest-Api](https://github.com/zEhmsy/Arduino-Digest-Api)

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- ACKNOWLEDGMENTS -->

## Acknowledgments

* [RFC 2617](https://tools.ietf.org/html/rfc2617)
* [Arduino Forum](https://forum.arduino.cc/)

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->

<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[contributors-shield]: https://img.shields.io/github/contributors/zEhmsy/Arduino-Digest-Api.svg?style=for-the-badge
[contributors-url]: https://github.com/zEhmsy/Arduino-Digest-Api/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/zEhmsy/Arduino-Digest-Api.svg?style=for-the-badge
[forks-url]: https://github.com/zEhmsy/Arduino-Digest-Api/network/members
[stars-shield]: https://img.shields.io/github/stars/zEhmsy/Arduino-Digest-Api.svg?style=for-the-badge
[stars-url]: https://github.com/zEhmsy/Arduino-Digest-Api/stargazers
[issues-shield]: https://img.shields.io/github/issues/zEhmsy/Arduino-Digest-Api.svg?style=for-the-badge
[issues-url]: https://github.com/zEhmsy/Arduino-Digest-Api/issues
[license-shield]: https://img.shields.io/github/license/zEhmsy/Arduino-Digest-Api.svg?style=for-the-badge
[license-url]: https://github.com/zEhmsy/Arduino-Digest-Api/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/g-turturro
[product-screenshot]: product_screenshot.PNG
