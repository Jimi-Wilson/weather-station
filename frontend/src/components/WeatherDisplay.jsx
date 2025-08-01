import {useState, useEffect} from "react";
import WeatherBlocks from "./WeatherBlocks.jsx";
import axios from 'axios';
import GraphSection from "./GraphSection.jsx";


const apiKey = import.meta.env.VITE_API_KEY;

if (apiKey) {
    axios.defaults.headers.common['Authorization'] = `Api-Key ${apiKey}`;
}

const getFormattedDate = (date) => date.toISOString().split('T')[0];

function WeatherDisplay() {

    const [latestWeatherData, setLatestWeatherData] = useState(null);
    const [historicalWeatherData, setHistoricalWeatherData] = useState(null);

    const [isLoading, setIsLoading] = useState(true);
    const [error, setError] = useState(null);
    
    let period = 1;

    const [startDate, setStartDate] = useState(() => {
        const date = new Date();
        date.setDate(date.getDate() - period);
        return date;
    });
    const [endDate, setEndDate] = useState(new Date());

    const POLLING_INTERVAL = 10000;

    useEffect(() => {
        const fetchLatestData = async () => {
            try {
                const response = await axios.get(`/api/weather/latest/`);
                setLatestWeatherData(response.data);
            } catch (error) {
                console.error('An error occurred when loading the latest' +
                    ' weather data:' + error);
            }
        }

        fetchLatestData();

        const interval = setInterval(fetchLatestData, POLLING_INTERVAL);
        return () => clearInterval(interval);
    }, []);

    useEffect(() => {
        const fetchHistoricalData = async () => {
            setIsLoading(true);
            setError(null);

            try {
                const response = await axios.get(`/api/weather/range?start_datetime=${startDate.toISOString()}&end_datetime=${endDate.toISOString()}`);
                setHistoricalWeatherData(response.data);
            } catch (error) {
                console.error('An error occurred when loading the historical' +
                    ' weather data:' + error);
                setError(error);
            } finally {
                setIsLoading(false);
            }
        }

        fetchHistoricalData();
    }, [startDate, endDate])

    useEffect(() => {
        if (!latestWeatherData || !historicalWeatherData?.results || historicalWeatherData.results.length === 0) {
            return;
        }

        const isViewingToday = getFormattedDate(endDate) === getFormattedDate(new Date());

        if (isViewingToday) {
           const lastTimestampInHistoricalData = new Date(historicalWeatherData.results[historicalWeatherData.results.length - 1].timestamp);
           const latestTimestamp = new Date(latestWeatherData.timestamp);

           if (latestTimestamp > lastTimestampInHistoricalData) {
               setHistoricalWeatherData(prevData => ({
                   ...prevData,
                   results: [...prevData.results, latestWeatherData]
               }));
           }
        }
    }, [endDate, latestWeatherData])


    const changePeriod = (period) => {
        const newEndDate = new Date();
        const newStartDate = new Date();

        newStartDate.setDate(newStartDate.getDate() - period);
        setStartDate(newStartDate);
        setEndDate(newEndDate);
    }

    if (isLoading) {
        return;
    }

    if (error) {
        return <div>Error: {error.message}</div>
    }

    console.log(historicalWeatherData);

    return (
        <div>

            <WeatherBlocks latestWeatherData={latestWeatherData}/>
            <GraphSection weatherData={historicalWeatherData.results} changePeriod={changePeriod}/>
        </div>
    )
}

export default WeatherDisplay;
