const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
    <html>
        <head>
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <style>
                body { text-align: center; margin-top: 20%; }
                button { display: block; margin: 10px auto; padding: 8px 4px; min-width: 152px;}
            </style>
        </head  >
        <body onload="setRTC()">
            
            <div>
                <h2>Duck Entertainment System</h2>
                <p id="time-p"></p>
                <button onclick="syncRTC();">Sync RTC</button>
                <button onclick="calibrateIMU();">Calibrate IMU</button>
                <button onclick="downloadLog();">Download Last Log</button>
            </div>

            <script>
                async function syncRTC() {
                    let currentTime = new Date();
                    let json = {
                        "Y": currentTime.getFullYear(),
                        "M": currentTime.getMonth() + 1,
                        "D": currentTime.getDate(),
                        "h": currentTime.getHours(),
                        "m": currentTime.getMinutes(),
                        "s": currentTime.getSeconds()
                    };
                    try {
                        const response = await fetch('/setRTC', {
                            method: 'POST',
                            headers: {
                                'Content-Type': 'application/json'
                            },
                            body: JSON.stringify(json)
                            });
                        if (response.ok) {
                            alert("RTC sync!");
                            location.reload();
                        } else {
                            alert("Failed to sync RTC");
                        }
                    } 
                    catch (error) {
                        console.error('Error:', error);
                        alert("An error occurred");
                        }
                    }

                function setRTC() {
                    fetch('/setRTC', { method: 'GET' })
                        .then(response => {
                        if (response.ok) {
                            return response.json();
                        } else {
                            throw new Error('Failed to get RTC');
                        }
                        })
                        .then(data => {
                            document.getElementById('time-p').innerText = 'MCU onload date: ' + data.time;
                            // alert('RTC get successful');
                        })
                        .catch(error => {
                            console.error('Error:', error);
                            alert('Failed to get RTC');
                        });
                    }
            </script>
        </body>
    </html>
)rawliteral";
