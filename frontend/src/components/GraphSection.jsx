import { useMemo} from "react";
import {LineChart, CartesianGrid, XAxis, YAxis, Line, Legend, Tooltip, ResponsiveContainer } from 'recharts';

function GraphSection({ weatherData, changePeriod }){



    const orderedData = useMemo(() => {
        if (!weatherData || weatherData.length === 0) {
            return [];
        }

        return [...weatherData].reverse();
    }, [weatherData])


    const periodButtons = (
        <div style={{ textAlign: 'center', margin: '20px 0' }}>
            <button onClick={() => changePeriod(1)}>Last Day</button>
            <button onClick={() => changePeriod(7)} style={{ margin: '0 10px' }}>Last 7 Days</button>
            <button onClick={() => changePeriod(30)}>Last 30 Days</button>
        </div>
    );


    if (weatherData.length === 0) {
        return <div>
            No data to display
            { periodButtons }
        </div>;
    }


    const formattedTimestamps = (ticks) => {
        return new Date(ticks).toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
    }



    return (
        <div>
            {periodButtons}
            <ResponsiveContainer width="100%" height={400}>
                <LineChart data={orderedData}>
                    <CartesianGrid/>
                    <Line dataKey="temperature" type="monotone" stroke="#8884d8" dot={false} isAnimationActive={false} />
                    <XAxis dataKey="timestamp" tickFormatter={formattedTimestamps}/>
                    <YAxis />
                    <Tooltip />
                    <Legend />
                </LineChart>
            </ResponsiveContainer>
        </div>
    )
}


export default GraphSection;
