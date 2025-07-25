import {useState, useEffect} from "react";
import WeatherBlocks from "./WeatherBlocks.jsx";
import axios from 'axios';

function WeatherDisplay() {
    const [weatherData, setWeatherData] = useState(null);
    const [isLoading, setIsLoading] = useState(true);
    const [error, setError] = useState(null);

    const POLLING_INTERVAL = 2500;

    useEffect(() => {

        const fetchData = () => {
            axios.get('http://127.0.0.1:8000/api/weatherdata/')
                .then(function (response) {
                    console.log(response.data);
                    setWeatherData(response.data);
                })

                .catch(function (error) {
                    console.log("An error occurred when trying to fetch weather data: " + error);
                    setError(error);
                })

                .finally(function () {
                    setIsLoading(false);
                })
        }

        fetchData();

        const interval = setInterval(fetchData, POLLING_INTERVAL);

        return () => clearInterval(interval);
    }, []);

    if (isLoading) {
        return <div>Loading...</div>
    }

    if (error) {
        return <div>Error: {error.message}</div>
    }


    return (
        <div>
            <WeatherBlocks weatherData={weatherData}/>
        </div>
    )
}

export default WeatherDisplay;