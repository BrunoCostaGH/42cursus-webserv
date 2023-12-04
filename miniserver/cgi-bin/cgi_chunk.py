# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    cgi_chunk.py                                       :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maricard <maricard@student.porto.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/18 17:24:25 by maricard          #+#    #+#              #
#    Updated: 2023/12/04 14:42:20 by maricard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import os, sys

uploads_folder = os.environ.get('UPLOAD_STORE')

if (uploads_folder != "uploads") :
    location = "(check configuration file)"
else :
    location = uploads_folder

# Check if the folder exists, if not create it
if not os.path.exists(uploads_folder):
    os.makedirs(uploads_folder)

extensions = {
    'text/plain': '.txt',
    'image/jpeg': '.jpg',
    'image/png': '.png',
    'application/pdf': '.pdf',
    'application/json': '.json',
    'application/octet-stream': '.bin',
    'video/mp4': '.mp4',
    'audio/mpeg': '.mp3'
}

message = ''

if 'CONTENT_TYPE' in os.environ :

    if os.environ['CONTENT_TYPE'] in extensions:
    
        raw_data = sys.stdin.buffer.read()
        fn = 'output' + extensions[os.environ['CONTENT_TYPE']]
        upload_path = os.path.join(uploads_folder, fn)

        # Check if the file already exists, if not create it
        if not os.path.exists(upload_path):
            with open(upload_path, 'wb') as new_file:
                new_file.write(raw_data)

            message = f'The file "{fn}" was uploaded successfully to /{location}'
            header = "HTTP/1.1 201 Created\r\n"

        else:
            message = f'The file "{fn}" already exists'
            header = "HTTP/1.1 202 Accepted\r\n"
    
    else :
        message = f'The file type "{os.environ["CONTENT_TYPE"]}" is not supported'
        header = "HTTP/1.1 415 Unsupported Media Type\r\n"

else:
    message = 'No file was uploaded'
    header = "HTTP/1.1 400 Bad Request\r\n"

print (header + """
                <!DOCTYPE html>
                <html lang="en">

                <head>
                    <meta charset="UTF-8">
                    <meta http-equiv="X-UA-Compatible" content="IE=edge">
                    <meta name="viewport" content="width=device-width, initial-scale=1.0">
                    <title>POST</title>
                    <style>
                        body {
                            font-family: Arial, sans-serif;
                            display: flex;
                            justify-content: center;
                            align-items: center;
                            height: 100vh;
                            margin: 0;
                        }

                        .container {
                            text-align: center;
                            max-width: 400px;
                            width: 100%;
                        }

                        h1 {
                            font-size: 3em;
                            margin-bottom: 10px;
                        }

                        p {
                            font-size: 1.2em;
                            color: #888;
                            margin-bottom: 30px;
                        }

                        .upload-btn-wrapper {
                            position: relative;
                            overflow: hidden;
                            display: inline-block;
                            margin-bottom: 20px;
                            cursor: pointer;
                        }

                        .btn {
                            border: 2px solid gray;
                            color: gray;
                            background-color: white;
                            padding: 8px 20px;
                            border-radius: 8px;
                            font-size: 20px;
                            font-weight: bold;
                        }

                        .upload-btn-wrapper input[type=file] {
                            font-size: 100px;
                            position: absolute;
                            left: 0;
                            top: 0;
                            opacity: 0;
                            cursor: pointer;
                        }

                        .btn-submit {
                            font-size: 20px;
                            font-weight: bold;
                            background-color: #007bff;
                            color: white;
                            padding: 10px 20px;
                            border: none;
                            border-radius: 8px;
                            cursor: pointer;
                        }

                        .home-btn {
                            font-size: 20px;
                            background-color: #ffaa00;
                            color: white;
                            padding: 10px 20px;
                            border: none;
                            border-radius: 8px;
                            cursor: pointer;
                        }
                    </style>
                </head>
                <link rel="icon" href="/html_pages/verified.ico" type="image/x-icon">

                <body>
                    <div class="container">
                        <h1>Upload File</h1>
                        <p>Select a file from your device and press "Upload"</p>
                        <form action="/cgi-bin/cgi_post.py" method="POST" id="uploadForm" enctype="multipart/form-data" onsubmit="return validateForm()">
                            <div class="upload-btn-wrapper">
                                <button class="btn">Choose file</button>
                                <input type="file" name="filename" id="fileInput"/>
                            </div>
                            <br>
                            <div>""" + message + """</div>
                            <br>
                            <input class="btn-submit" type="submit" value="Upload">
                        </form>
                        <br>
                        <a href="/html_pages/home.html"><button class="home-btn"><b>Home Page</button></a>
                    </div>

                    <script>
                        function validateForm()
                        {
                            let fileInput = document.getElementById("fileInput");
                            let fileName = fileInput.value;
                            
                            if (!fileName)
                            {
                                alert("Please choose a file to upload");
                                return false;
                            }
                            return true;
                        }
                    </script>
                <body>
                </html>""")
